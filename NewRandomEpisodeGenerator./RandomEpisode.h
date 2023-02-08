#pragma once
#ifndef RandomEpisode_h
#define RandomEpisode_h
#include "string"

using namespace std;

string returnEpisode();
string showRecentEpisodes(int i);
string returnSeriesName();
string returnSeasonName();
string returnEpisodeName();


void openFile(string series, string season, string episode);
void updateRecentWatched();
int totalEpisodesViewed();
void writeToFile(string write);
void findViewed(string search, int ctr);
void getEpisodes();


#endif
