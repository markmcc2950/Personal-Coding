#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sstream>
#include <fstream>
#include <iostream>

#include <Windows.h>

#include "RandomEpisode.h"

bool searching = true;
int counter = 0;
int totalEpisodes = 0;
int numToShow = 5;
std::string recentEpisodesList[25];
//std::vector<std::string> recentEpisodesList;
std::string foundEpisode = "";
std::string seriesName = "";
std::string seasonName = "";
std::string episodeName = "";													// Name of the episode
std::string episodeNum = "";													// Number of the episode (Example: 05 TITLE)

/*

	TODO:	Need to save the values of the episode, season, and series to a global variable
			Use this information to be passed into the values to be read to grab the actual episode information for VLC player
			-	Find the episode name based on the episode int number given

*/
void setNumberToShow(int n) {
	numToShow = n;
}

int getNumberToShow() {
	return numToShow;
}

void clearWatchedList() {
	for (int i = 0; i < sizeof(recentEpisodesList)/sizeof(recentEpisodesList[0]); i++) {
		recentEpisodesList[i] = "";
	}
}

std::string returnEpisode() {
	return foundEpisode;
}

std::vector<std::string> showRecentEpisodes(int numToShow) {
	std::vector<std::string> myList;
	myList.clear();
	for (int i = 0; i < numToShow; i++) {
		myList.push_back(recentEpisodesList[i]);
	}
	return myList;
}

std::string returnSeriesName() {
	return seriesName;
}

std::string returnSeasonName() {
	return seasonName;
}

std::string returnEpisodeName() {												// Gets the episode name from the string gathered from the series' .txt file
	std::string tempEpisode = episodeName;
	size_t pos = episodeName.find("\"");
	tempEpisode = episodeNum + " " + episodeName.substr(pos + 1);

	if (!tempEpisode.empty()) {												// Remove the final quotation mark from the episode name
		tempEpisode.resize(tempEpisode.size() - 1);
	}

	tempEpisode += ".mkv";

	return tempEpisode;
}

void openFile(std::string series, std::string season, std::string episode) {
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

void updateRecentWatched() {
	std::ifstream brFile("EpisodesViewed.txt");
	int episodesViewed = totalEpisodesViewed();
	if (episodesViewed >= 2) {
		int totalRecord = 0;
		int ctr = 0;

		if (brFile.is_open()) {
			std::string line;
			while (std::getline(brFile, line) && totalRecord < numToShow) {
				if (ctr >= episodesViewed - numToShow && episodesViewed >= numToShow + 2) {
					recentEpisodesList[totalRecord] = line.c_str();
					totalRecord++;
				}


				ctr++;
			}
			brFile.close();
		}
	}
	for (int i = 0; i < numToShow; i++) {
		std::string tempString = recentEpisodesList[numToShow - 1 - i];
		recentEpisodesList[numToShow - 1 - i] = recentEpisodesList[i];
		recentEpisodesList[i] = tempString;
	}
}

int totalEpisodesViewed() {
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

void writeToFile(std::string write) {													// Called when needed to write the found episode
	std::ofstream brFile2;
	brFile2.open("EpisodesViewed.txt", std::ios_base::app);
	if (brFile2.is_open()) {
		brFile2 << write << "\n";
	}
	brFile2.close();
}

void findViewed(std::string search, int ctr) {
	int viewedCounter = 0;
	int counter = 0;


	std::string search2 = search;
	std::ifstream brFile("EpisodesViewed.txt");													// Open EpisodesViewed file to check if episode's been viewed
	std::string line;
	if (brFile.is_open()) {
		while (std::getline(brFile, line) && searching) {
			counter++;																// Counts how many lines have been viewed
			if (line.find(search, 0) != std::string::npos) {								// If there's a match, increase counter to stop searching for this RNG
				viewedCounter++;
			}
			if (viewedCounter >= 1) {												// Stop searching if duplicate is found
				break;
			}
			else if (viewedCounter == 0 && counter >= (ctr)) {
				foundEpisode = search;
				brFile.close();
				writeToFile(search);
				searching = false;													// Stop searching
				break;																// Stops the loop of searching through files
			}
		}
	}
}

void getEpisodes() {
	int HIMYM[9];																// How I Met Your Mother
	int PnR[7];																	// Parks & Recreation
	int RnM[3];																	// Rick & Morty
	int series = 0; int season = 0; int episode = 0;
	searching = true;


	HIMYM[0] = 22;
	HIMYM[1] = 22;
	HIMYM[2] = 20;
	HIMYM[3] = 24;
	HIMYM[4] = 24;
	HIMYM[5] = 24;
	HIMYM[6] = 24;
	HIMYM[7] = 24;
	HIMYM[8] = 24;

	PnR[0] = 6;
	PnR[1] = 24;
	PnR[2] = 16;
	PnR[3] = 22;
	PnR[4] = 22;
	PnR[5] = 22;
	PnR[6] = 13;

	RnM[0] = 11;
	RnM[1] = 10;
	RnM[2] = 10;

	for (int i = 0; i < 9; i++) {
		totalEpisodes += HIMYM[i];
	}
	for (int i = 0; i < 7; i++) {
		totalEpisodes += PnR[i];
	}
	for (int i = 0; i < 3; i++) {
		totalEpisodes += RnM[i];
	}

	//cout << totalEpisodes << endl;
	srand(time(0));

	/*
		Parse through EpisodesViewed.txt to find amount of total lines
		We use this lines total to know when to stop scanning for viewed episodes.
		This functionality is used in void findViewed()
	*/
	std::ifstream FileBR("EpisodesViewed.txt");
	std::ifstream FileHIMYM, FilePnR, FileRnM;
	std::string lineBR, line, search;

	int BRCounter = totalEpisodesViewed();

	while (searching) {
		//series = rand() % 3;													// Temporarily commented out to only search for Parks & Rec for testing
		series = 1;
		//cout << series << endl;
		if (series == 0) {														// If series is How I Met Your Mother
			season = rand() % 9 + 1;
			episode = rand() % HIMYM[season - 1] + 1;							// Find the HIMYM episode		

			// Temporary for testing
			seriesName = "How I Met Your Mother";
		}
		else if (series == 1) {													// If series is Parks & Recreation
			season = rand() % 7 + 1;
			episode = rand() % PnR[season - 1] + 1;								// Find the Parks & Rec episode

			seriesName = "Parks & Recreation";
		}
		else if (series == 2) {
			season = rand() % 3 + 1;
			episode = rand() % RnM[season - 1] + 1;

			// Temporary for testing
			seriesName = "Rick & Morty";
		}

		// Get the episodeName value for the string
		if (episode < numToShow) {
			episodeNum = "0" + std::to_string(episode);
		}
		else {
			episodeNum = std::to_string(episode);
		}
		seasonName = "Season " + std::to_string(season);								// Get the season in string form for searching the directory later



		std::ostringstream episodeSearch;											// Season: #, Episode: #
		episodeSearch << "Season: " << season << ", Episode: "					// Store found season and episode to episodeSearch string
			<< episode;

		if (series == 0) {
			FileHIMYM.open("HIMYM.txt");										// Open HIMYM file to check episode names
		}
		else if (series == 1) {
			FilePnR.open("PnR.txt");											// Open HIMYM file to check episode names
		}
		else if (series == 2) {
			FileRnM.open("RnM.txt");
		}

		bool found = false;

		if (FileHIMYM.is_open() && !found) {
			search = episodeSearch.str();

			while (getline(FileHIMYM, line)) {										// While searching through the file
				if (line.find(search, 0) != std::string::npos) {							// If match is found
					episodeName = (std::string)line.c_str();								// Save the c_str() for use later on to gather solely the episode name

					foundEpisode = "How I Met Your Mother:\t\t";
					foundEpisode.append(line.c_str());

					FileHIMYM.close();
					findViewed(foundEpisode, BRCounter);
				}
			}
		}
		else if (FilePnR.is_open() && !found) {
			search = episodeSearch.str();

			while (getline(FilePnR, line)) {										// While searching through the file
				if (line.find(search, 0) != std::string::npos) {							// If match is found
					episodeName = (std::string)line.c_str();								// Save the c_str() for use later on to gather solely the episode name

					foundEpisode = "Parks & Recreation:\t\t\t";
					foundEpisode.append(line.c_str());								// Episode found, write to line string

					FilePnR.close();
					findViewed(foundEpisode, BRCounter);
				}
			}
		}
		else if (FileRnM.is_open() && !found) {
			search = episodeSearch.str();

			while (getline(FileRnM, line)) {										// While searching through the file
				if (line.find(search, 0) != std::string::npos) {							// If match is found
					episodeName = (std::string)line.c_str();								// Save the c_str() for use later on to gather solely the episode name

					foundEpisode = "Rick & Morty:\t\t\t";
					foundEpisode.append(line.c_str());								// Episode found, write to line string

					FileRnM.close();
					findViewed(foundEpisode, BRCounter);
				}
			}
		}
	}
}