// Source: https://www.youtube.com/watch?v=FOIbK4bJKS8

#include "cMain.h"
#include <string>
#include "shobjidl_core.h"
#include <iostream>
#include <windows.h>
#include <shlobj.h>

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_BUTTON(10001, OnRandomButtonClicked)
EVT_BUTTON(10002, OnRandomButtonClicked)
EVT_BUTTON(10003, onBrowseButtonClicked)
wxEND_EVENT_TABLE()

wxListBox m_list3;
RandomEpisode re;
extern DirectoryHandler dh;

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Random Episode Generator", wxPoint(0, 0), wxSize(1280, 720)) 
{
	// Create our buttons
	m_btn1 = new wxButton(this, 10001, "Generate Random Episode", wxPoint(10, 10), wxSize(150, 30));
	m_btn2 = new wxButton(this, 10002, "Play Next Episode", wxPoint(10, 40), wxSize(150, 30));
	m_btn3 = new wxButton(this, 10003, "Browse", wxPoint(600, 10), wxSize(75, 30));
	m_btn1->Disable();
	
	// Create our string boxes
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(500, 20));
	m_list2 = new wxListBox(this, wxID_ANY, wxPoint(10, 150), wxSize(500, 160));

	// Create the directory string box, but hide it
	m_list3 = new wxListBox(this, wxID_ANY, wxPoint(600, 110), wxSize(1, 20));
	m_list3->Hide();
	

	// Populate the recently watched list
	re.updateRecentWatched();
	for (int i = 0; i < 10; i++) {
		episodeList[i] = re.showRecentEpisodes(i);
		if (episodeList[i] != "" || episodeList[i] != "ALL EPISODES VIEWED:") {
			m_list2->AppendString(episodeList[i]);
		}		
	}
}

cMain::~cMain() 
{
	DestroyButton(m_btn1);
	DestroyButton(m_btn2);
	DestroyButton(m_btn3);
	DestroyListBox(m_list1);
	DestroyListBox(m_list2);
}

void cMain::onBrowseButtonClicked(wxCommandEvent& evt)
{
	// Initialize the BROWSEINFO structure
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(bi));

	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

	// Display the "Browse for Folder" dialog box
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != NULL)
	{
		TCHAR path[MAX_PATH];
		if (SHGetPathFromIDList(pidl, path))
		{
			std::cout << "Selected folder: " << path << std::endl;
			std::string newPath = re.tcharToString(path);
			std::cout << newPath << std::endl;
			dh.setDirectory(newPath);

			m_btn1->Enable();
		}
		else
		{
			std::cout << "Error: Failed to retrieve folder path." << std::endl;
		}
		CoTaskMemFree(pidl);
	}
	else
	{
		std::cout << "No folder selected." << std::endl;
	}

	std::string directoryPath = dh.getDirectory();
	int pathLength = directoryPath.length();
	pathLength = pathLength * 5 + 40;

	if (m_list3 != nullptr) {
		m_list3->Clear();
	}
	if (pathLength > 0) {
		m_list3->SetSize(pathLength, 20);
		m_list3->AppendString(directoryPath);	
		m_list3->Show();
	}
	
	// Visually display the path selected
	//m_list3 = new wxListBox(this, wxID_ANY, wxPoint(600, 110), wxSize(pathLength, 20));
	
}

void cMain::OnRandomButtonClicked(wxCommandEvent& evt)
{
	// Get the number of folders within the selected directory
	std::string selectedDirectory = dh.getDirectory();
	bool isValidFolder = true;

	if (selectedDirectory == "") {
		// Display an error, and show this error in the Directory path under the "BROWSE" button
		std::cerr << "ERROR: No directory selected." << std::endl;
		std::string errorDisplay = "ERROR! Select a directory and try again";

		m_list3->SetSize(errorDisplay.length() * 5 + 40, 20);
		m_list3->AppendString(errorDisplay);
		m_list3->Show();
		isValidFolder = false;
	}

	int directoryCount = 0;
	int fileCount = 0;
	int randomValue = 0;
	int loopCounter = 0;
	int maxLoop = 0;
	

	srand(time(0));

	m_btn1->Disable();

	while (isValidFolder) {		
		directoryCount = dh.getDirectoryFolderCount(selectedDirectory);
		if (directoryCount > 0) {																	// Append name to directory
			rand() % 2 ? randomValue = rand() % directoryCount : randomValue = re.getRandomNumber(directoryCount);

			std::string nextFolder = re.getRandomFolder(selectedDirectory, randomValue);			// Get name of randomly chosen folder
			selectedDirectory += "//" + nextFolder;
		}
		else {																						// If we return no more folders
			fileCount = dh.getNumFilesInFolder(selectedDirectory);
			if (fileCount > 0) {
				randomValue = rand() % fileCount;
				selectedDirectory += "//Episode " + std::to_string(fileCount);

				// Format our string prior to checking with the written file
				selectedDirectory = dh.formatFinalDirectory(selectedDirectory, "//", "\\", false);
				selectedDirectory = dh.formatFinalDirectory(selectedDirectory, dh.getDirectory() + "\\", "", false);
				selectedDirectory = dh.formatFinalDirectory(selectedDirectory, "\\", "", true);

				// Returns false if we HAVEN'T watched it yet
				if (!re.setEpisodeToView(selectedDirectory)) {
					isValidFolder = false;

					m_list3->Clear();
					m_list3->SetSize(selectedDirectory.length() * 5 + 40, 20);
					m_list3->AppendString(selectedDirectory);
					m_list3->Show();

					m_list1->Clear();
					m_list1->AppendString(selectedDirectory);

					// Don't show the first episode, but update the list after each episode
					if (selectedDirectory != "") {
						m_list2->Clear();
						for (int i = 9; i > 0; i--) {
							episodeList[i] = episodeList[i - 1];
						}
						episodeList[0] = selectedDirectory;
						for (int i = 0; i < 10; i++) {
							m_list2->AppendString(episodeList[i]);
						}
					}

					maxLoop = 0;
					m_btn1->Enable();																// Re-Enable the button to be able to search again

					break;
				}
				// If we have watched it, reset and continue searching
				else {
					isValidFolder = true;
					selectedDirectory = dh.getDirectory();
					loopCounter = -1;
				}
			}
			// We selected an empty folder, reset search
			else {
				isValidFolder = true;
				selectedDirectory = dh.getDirectory();
				loopCounter = 0;
			}
		}

		++loopCounter;
		if (loopCounter >= 10 && isValidFolder) {													// If we're too many folders in, reset
			isValidFolder = true;
			selectedDirectory = dh.getDirectory();
			loopCounter = 0;
		}

		++maxLoop;
		if (maxLoop >= 10000) {
			std::string errorDisplay = "ERROR! Unable to find an episode: " + selectedDirectory;

			m_list1->Clear();
			m_list1->AppendString(errorDisplay);

			m_btn1->Enable();																	// Re-Enable the button to be able to search again
			maxLoop = 0;
			break;
		}
	}

	m_btn1->Enable();

	//openFile(seriesTitle, seasonTitle, episodeTitle);
	evt.Skip();
}

void cMain::OnNextButtonClicked(wxCommandEvent& evt) {
	m_list1->Clear();
	//re.getEpisodes();
	std::string episodeToView = re.returnEpisode();

	m_list1->AppendString(episodeToView);

	// Don't show the first episode, but update the list after each episode
	if (recentEpisode != "") {
		episodePicked = true;
	}
	// Grab the list of recent episodes (from most recently watched first)
	if (episodePicked) {
		m_list2->Clear();
		for (int i = 9; i > 0; i--) {
			episodeList[i] = episodeList[i - 1];
		}
		episodeList[0] = recentEpisode;
		for (int i = 0; i < 10; i++) {
			m_list2->AppendString(episodeList[i]);
		}
	}
	recentEpisode = re.returnEpisode();

	// Get the strings of information to parse the directory and open the video file
	std::string seriesTitle = re.returnSeriesName();
	std::string seasonTitle = re.returnSeasonName();
	std::string episodeTitle = re.returnEpisodeName();

	re.openFile(seriesTitle, seasonTitle, episodeTitle);
	evt.Skip();
}

void cMain::DestroyListBox(wxListBox* listBox) {
	listBox->Destroy();
}

void cMain::DestroyButton(wxButton* button) {
	button->Destroy();
}