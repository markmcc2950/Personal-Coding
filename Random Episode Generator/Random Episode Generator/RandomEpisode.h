#pragma once
#ifndef RandomEpisode_h
#define RandomEpisode_h
#include <string>

#include "DirectoryHandler.h"

class RandomEpisode {
public:
	std::string tcharToString(TCHAR toConvert[]);
	std::string returnEpisode();
	std::string showRecentEpisodes(int i);
	std::string returnSeriesName();
	std::string returnSeasonName();
	std::string returnEpisodeName();

	int getRandomNumber(int n);														// For quickly grabbing a random value while parsing directories
	std::string getRandomFolder(std::string selectedFolder, int folderIndex);
	std::string getRandomSeasonNumber();
	std::string getRandomEpisodeName();

	void openFile(std::string series, std::string season, std::string episode);
	void updateRecentWatched();
	int totalEpisodesViewed();
	void writeToFile(std::string write);
	bool findViewed(std::string search, int ctr);
	void getEpisodes();

	bool setEpisodeToView(static std::string& episode);
private:
	bool searching = true;
	int counter = 0;
	int totalEpisodes = 0;
	std::string recentEpisodesList[10];
	std::string foundEpisode = "";
	std::string seriesName = "";
	std::string seasonName = "";
	std::string episodeName = "";													// Name of the episode
	std::string episodeNum = "";													// Number of the episode (Example: 05 TITLE)
};







#endif
