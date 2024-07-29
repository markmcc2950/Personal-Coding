#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sstream>
#include <fstream>
#include <iostream>

#include <windows.h>
#include <tchar.h>
#include <string>

#include "RandomEpisode.h"

extern DirectoryHandler dh;

/*

	TODO:	Need to save the values of the episode, season, and series to a global variable
			Use this information to be passed into the values to be read tto grab the actual episode information for VLC player
			-	Find the episode name based on the episode int number given

*/

std::string RandomEpisode::tcharToString(TCHAR toConvert[]) {
	TCHAR tstr[] = _T("Hello World"); // sample TCHAR string
	int tstr_len = _tcslen(toConvert); // get the length of TCHAR string

	char* buffer = new char[tstr_len + 1]; // create buffer to hold the converted string

	// Convert TCHAR to char using the WideCharToMultiByte function
	WideCharToMultiByte(CP_UTF8, 0, toConvert, -1, buffer, tstr_len, nullptr, nullptr);
	buffer[tstr_len] = '\0'; // add null terminator to the converted string

	std::string str(buffer); // create std::string from converted char buffer

	delete[] buffer; // free memory used by buffer

	return str;
}

std::string RandomEpisode::returnEpisode() {
	return foundEpisode;
}

std::string RandomEpisode::showRecentEpisodes(int i) {
	return recentEpisodesList[i];
}

std::string RandomEpisode::returnSeriesName() {
	return seriesName;
}

std::string RandomEpisode::returnSeasonName() {
	return seasonName;
}

std::string RandomEpisode::returnEpisodeName() {												// Gets the episode name from the string gathered from the series' .txt file
	std::string tempEpisode = episodeName;
	size_t pos = episodeName.find("\"");
	tempEpisode = episodeNum + " " + episodeName.substr(pos + 1);

	if (!tempEpisode.empty()) {												// Remove the final quotation mark from the episode name
		tempEpisode.resize(tempEpisode.size() - 1);
	}

	tempEpisode += ".mkv";

	return tempEpisode;
}

// -----------------------------------------------------------------------------------------

int RandomEpisode::getRandomNumber(int n) {
	srand(time(0));

	return rand() % n;
}

std::string RandomEpisode::getRandomFolder(std::string selectedFolder, int folderIndex) {
	return dh.getFolderByIndex(selectedFolder, folderIndex);
}

std::string RandomEpisode::getRandomSeasonNumber() {
	return "";
}

std::string RandomEpisode::getRandomEpisodeName() {
	return "";
}

// -----------------------------------------------------------------------------------------

void RandomEpisode::openFile(std::string series, std::string season, std::string episode) {
	// Save episode information for usage
	std::string seriesName = series;
	std::string seasonName = season;
	std::string episodeName = episode;

	// Get the application (VLC) to use
	const char* appname = "C:\\Program Files\\VideoLAN\\VLC\\vlc.exe";

	// Get the file directory path using the series, season and episode strings passed in
	//string fileNameBefore = "H:\\Users\\Mark\\Videos\\TV\\" + seriesName + "\\" + seasonName + "\\" + episodeName;
	std::string fileNameBefore = "G:\\" + seriesName + "\\" + seasonName + "\\" + episodeName;

	// Save the file name to a const char*
	const char* fileName = &fileNameBefore[0];

	STARTUPINFOA startupInfo;
	PROCESS_INFORMATION processInfo;
	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	memset(&processInfo, 0, sizeof(processInfo));

	char buf[MAX_PATH + 300];
	wsprintfA(buf, "%s \"%s\" --play-and-exit", appname, fileName);
	CreateProcessA(0, buf, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
}

void RandomEpisode::updateRecentWatched() {
	std::ifstream brFile("EpisodesViewed.txt");
	int episodesViewed = totalEpisodesViewed();
	if (episodesViewed >= 2) {
		int totalRecord = 0;
		int ctr = 0;

		if (brFile.is_open()) {
			std::string line;
			while (std::getline(brFile, line) && totalRecord < 10) {
				if (ctr >= episodesViewed - 10 && episodesViewed >= 12) {
					recentEpisodesList[totalRecord] = line.c_str();
					totalRecord++;
				}
				
				
				ctr++;
			}
			brFile.close();
		}
	}
	for (int i = 0; i < 5; i++) {
		std::string tempString = recentEpisodesList[9 - i];
		recentEpisodesList[9 - i] = recentEpisodesList[i];
		recentEpisodesList[i] = tempString;
	}
}

int RandomEpisode::totalEpisodesViewed() {
	/*
		This section checks the EpisodesaViewed.txt file to see how many episodes have been viewed.
		If all have been viewed, it stops the program and ends the search.
	*/
	std::ifstream FileBR("EpisodesViewed.txt");
	std::ifstream FileHIMYM, FilePnR, FileRnM;
	std::string lineBR, line, search;

	int BRCounter = 0;
	if (FileBR.is_open()) {
		while (std::getline(FileBR, lineBR)) {
			BRCounter++;
		}
		if (BRCounter >= totalEpisodes) {
			// cout << "All episodes viewed!" << endl;
			searching = false;
		}
		float percent = (float)(BRCounter) / (float)(totalEpisodes);
		// cout << "\t\tNumber of Episodes Watched:\t" << BRCounter - 1 << "/" << totalEpisodes << " (" << (percent) * 100 << " %)" << endl;
		FileBR.close();
	}
	return BRCounter;
}

void RandomEpisode::writeToFile(std::string write) {													// Called when needed to write the found episode
	std::ofstream brFile2;
	brFile2.open("EpisodesViewed.txt", std::ios_base::app);
	if (brFile2.is_open()) {
			brFile2 << write << "\n";
	}
	brFile2.close();
}

bool RandomEpisode::findViewed(std::string search, int ctr) {
	int counter = 0;	

	std::string search2 = search;
	std::ifstream episodeFile("EpisodesViewed.txt");								// Open EpisodesViewed file to check if episode's been viewed

	if (!episodeFile.is_open()) {
		std::cerr << "Unable to open EpisodesViewed.txt" << std::endl;
		return false;
	}

	std::string line = "";

	while (std::getline(episodeFile, line) && searching) {
		++counter;																// Counts how many lines have been viewed
		if (line.find(search, 0) != std::string::npos) {						// If there's a match, return true
			episodeFile.close();
			return true;
			break;
		}
		else if (counter >= (ctr)) {
			foundEpisode = search;
			episodeFile.close();
			writeToFile(search);
			searching = false;													// Stop searching
			return false;														// Stops the loop of searching through files
			break;
		}
	}

	episodeFile.close();
	return false;
}

bool RandomEpisode::setEpisodeToView(static std::string& episode) {
	int episodeCounter = totalEpisodesViewed();
	searching = true;
	return findViewed(episode, episodeCounter);
}