#include "App.h"
#include "RandomEpisode.h"
#include <tchar.h>
#include <stdlib.h>

#include <windows.h>
#include <atlstr.h>
#include <string>
#include <vector>

void mainApp() {
	setNumberToShow(5);
	clearWatchedList();
	updateRecentWatched();						// Get the information of our watch list
}

void showRecentList() {
	//int episodesToShow = getNumberToShow();
}