#include "DirectoryHandler.h"

#include <windows.h>
#include <filesystem>
#include <iostream>
#include <string>

DirectoryHandler dh;

std::string DirectoryHandler::getDirectory() {
	return folderPath;
}

void DirectoryHandler::setDirectory(std::string newfolderpath) {
	folderPath = newfolderpath;
}

int DirectoryHandler::getDirectoryFolderCount(const std::string& directoryPath) {
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    // Adjust the search path to include a wildcard to find all items in the directory
    std::string searchPath = directoryPath + "\\*";
    hFind = FindFirstFileA(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "FindFirstFile failed for path: " << directoryPath << " with error: " << GetLastError() << std::endl;
        return -1;
    }

    int folderCount = 0;

    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            std::string folderName = findFileData.cFileName;

            // Skip the "." and ".." directories
            if (folderName != "." && folderName != "..") {
                ++folderCount;
            }
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
    return folderCount;
}

int DirectoryHandler::getNumFilesInFolder(const std::string& directoryPath) {
    int fileCount = 0;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (std::filesystem::is_regular_file(entry.status())) {
                auto extension = entry.path().extension();
                if (
                    extension == ".mkv" || 
                    extension == ".wav" || 
                    extension == ".mp4" ||
                    extension == ".wmv" ||
                    extension == ".avi") {
                    ++fileCount;
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return fileCount;
}

std::string DirectoryHandler::getFolderByIndex(const std::string& directoryPath, int index) {
    int currentIndex = 0;

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_directory()) {
            if (currentIndex == index) {
                return entry.path().filename().string();
            }
            ++currentIndex;
        }
    }

    // Return an empty string if index is out of bounds
    return "";
}

std::string DirectoryHandler::getFileByIndex(const std::string& directoryPath, int index) {
    return "";
}

std::string DirectoryHandler::formatFinalDirectory(const std::string& input, const std::string& target, const std::string& replacement, const bool isFinalFormat) {
    std::string result = input;
    std::string::size_type pos = 0;
    std::string tempReplacement = replacement;

    int counter = 0;                                                            // Checks for replacing with a colon or a comma (only in final formatting)

    // Replace all instances of target with replacement
    while ((pos = result.find(target, pos)) != std::string::npos) {
        if (isFinalFormat) {
            counter == 0 ? tempReplacement = ": " : tempReplacement = ", ";

            ++counter;                                                          // Increment only in final formatting, tells us to replace differently
        }
        result.replace(pos, target.length(), tempReplacement);
        pos += tempReplacement.length();                                        // Move past the replacement
    }

    return result;
}