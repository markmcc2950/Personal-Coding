#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

bool searching = true;

void writeToFile(string write) {													// Called when needed to write the found episode
	ofstream viewFile2;
	viewFile2.open("Viewed.txt", ios_base::app);	
	if (viewFile2.is_open()) {\
		viewFile2 << write << "\n";
	}
	viewFile2.close();
}

void findViewed(string search, int ctr) {
	int viewedCounter = 0;
	int counter = 0;

	string search2 = search;
	ifstream viewFile("Viewed.txt");												// Open viewed file to check if episode's been viewed
	string line;
	if (viewFile.is_open()) {
		while (getline(viewFile, line) && searching) {
			counter++;																// Counts how many lines have been viewed
			if (line.find(search, 0) != string::npos) {								// If there's a match, increase counter to stop searching for this RNG
				viewedCounter++;
			}
			if (viewedCounter >= 1) {												// Stop searching if duplicate is found
				break;
			}
			else if (viewedCounter == 0 && counter >= (ctr)) {
				cout << search << endl;
				viewFile.close();
				writeToFile(search);				
				searching = false;													// Stop searching
				break;																// Stops the loop of searching through files
			}
		}
	}
}

int main() {
	int HIMYM[9][1];																// How I Met Your Mother
	int PnR[7][1];																	// Parks & Recreation
	int RnM[3][1];																	// Rick & Morty
	int totalEpisodes = 0;
	HIMYM[0][0] = 22;																// Season 1
	HIMYM[1][0] = 22;																// Season 2
	HIMYM[2][0] = 20;																// Season 3
	HIMYM[3][0] = 24;																// Season 4
	HIMYM[4][0] = 24;																// Season 5
	HIMYM[5][0] = 24;																// Season 6
	HIMYM[6][0] = 24;																// Season 7
	HIMYM[7][0] = 24;																// Season 8
	HIMYM[8][0] = 24;																// Season 9

	PnR[0][0] = 6;
	PnR[1][0] = 24;
	PnR[2][0] = 16;
	PnR[3][0] = 22;
	PnR[4][0] = 22;
	PnR[5][0] = 22;
	PnR[6][0] = 13;

	RnM[0][0] = 11;
	RnM[1][0] = 10;
	RnM[2][0] = 10;


	bool found = false;																// True if found
	
	bool matching = false;
	int counter = 0;
	int series = 0; int season = 0; int episode = 0;	
	string foundEpisode;

	for (int i = 0; i < 9; i++) {
		totalEpisodes += HIMYM[i][0];
	}
	for (int i = 0; i < 7; i++) {
		totalEpisodes += PnR[i][0];
	}
	for (int i = 0; i < 3; i++) {
		totalEpisodes += RnM[i][0];
	}

	//cout << totalEpisodes << endl;
	srand(time(0));

	/*
		Parse through Viewed.txt to find amount of total lines
		We use this lines total to know when to stop scanning for viewed episodes.
		This functionality is used in void findViewed()
	*/
	ifstream FileViewed("Viewed.txt");
	ifstream FileHIMYM, FilePnR, FileRnM;
	string lineViewed, line, search;

	int viewCounter = 0;

	/*
		This section checks the Viewed.txt file to see how many episodes have been viewed.
		If all have been viewed, it stops the program and ends the search.
	*/
	if (FileViewed.is_open()) {
		while (getline(FileViewed, lineViewed)) {
			viewCounter++;
		}
		if (viewCounter >= totalEpisodes) {
			cout << "All episodes viewed!" << endl;
			searching = false;
		}
		float percent = (float)(viewCounter)/(float)(totalEpisodes);
		cout << "\t\tNumber of Episodes Watched:\t" << viewCounter - 1 << "/" << totalEpisodes << " (" << (percent) * 100 << " %)" << endl;
		FileViewed.close();
	}
	

	while (searching) {
		series = rand() % 3;
		//cout << series << endl;
		if (series == 0) {															// If series is How I Met Your Mother
			season = rand() % 9 + 1;
			episode = rand() % HIMYM[season - 1][0] + 1;							// Find the HIMYM episode
		}
		else if (series == 1) {														// If series is Parks & Recreation
			season = rand() % 7 + 1;
			episode = rand() % PnR[season - 1][0] + 1;								// Find the Parks & Rec episode
		}
		else if (series == 2) {
			season = rand() % 3 + 1;
			episode = rand() % RnM[season - 1][0] + 1;
		}
		
		
		
		ostringstream episodeSearch;												// Season: #, Episode: #
		episodeSearch << "Season: " << season << ", Episode: "						// Store found season and episode to episodeSearch string
			<< episode;

		if (series == 0) {
			FileHIMYM.open("HIMYM.txt");											// Open HIMYM file to check episode names
		}
		else if (series == 1) {
			FilePnR.open("PnR.txt");												// Open HIMYM file to check episode names
		}
		else if (series == 2) {
			FileRnM.open("RnM.txt");
		}
		

		if (FileHIMYM.is_open() && !found) {
			search = episodeSearch.str();

			while (getline(FileHIMYM, line)) {										// While searching through the file
				if (line.find(search, 0) != string::npos) {							// If match is found
					foundEpisode = "How I Met Your Mother:\t\t";
					foundEpisode.append(line.c_str());
					//foundEpisode = line.c_str();									// Episode found, write to line string
					FileHIMYM.close();					
					findViewed(foundEpisode, viewCounter);
				}
			}
		}
		else if (FilePnR.is_open() && !found) {
			search = episodeSearch.str();

			while (getline(FilePnR, line)) {										// While searching through the file
				if (line.find(search, 0) != string::npos) {							// If match is found
					foundEpisode = "Parks & Recreation:\t\t";
					foundEpisode.append(line.c_str());								// Episode found, write to line string
					FilePnR.close();
					findViewed(foundEpisode, viewCounter);
				}
			}
		}
		else if (FileRnM.is_open() && !found) {
			search = episodeSearch.str();

			while (getline(FileRnM, line)) {										// While searching through the file
				if (line.find(search, 0) != string::npos) {							// If match is found
					foundEpisode = "Rick & Morty:\t\t\t";
					foundEpisode.append(line.c_str());								// Episode found, write to line string
					FileRnM.close();
					findViewed(foundEpisode, viewCounter);
				}
			}
		}
	}
}