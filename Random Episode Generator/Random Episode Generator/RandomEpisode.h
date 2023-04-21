#pragma once
#ifndef RandomEpisode_h
#define RandomEpisode_h
#include "string"
#include "vector"

void setNumberToShow(int n);
int getNumberToShow();
void clearWatchedList();
std::string returnEpisode();
std::vector<std::string> showRecentEpisodes(int numToShow);
std::string returnSeriesName();
std::string returnSeasonName();
std::string returnEpisodeName();


void openFile(std::string series, std::string season, std::string episode);
void updateRecentWatched();
int totalEpisodesViewed();
void writeToFile(std::string write);
void findViewed(std::string search, int ctr);
void getEpisodes();


#endif
