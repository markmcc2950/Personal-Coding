// Source: https://www.youtube.com/watch?v=FOIbK4bJKS8

#include "cMain.h"
#include "string.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Random Episode Generator", wxPoint(0, 0), wxSize(640, 480)) 
{
	m_btn1 = new wxButton(this, 10001, "Generate Random Episode", wxPoint(10, 10), wxSize(150, 50));
	//m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 650), wxSize(500, 30));
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(500, 20));
	m_list2 = new wxListBox(this, wxID_ANY, wxPoint(10, 150), wxSize(500, 160));

	// Populate the recently watched list
	updateRecentWatched();
	for (int i = 0; i < 10; i++) {
		episodeList[i] = showRecentEpisodes(i);
		if (episodeList[i] != "" || episodeList[i] != "ALL EPISODES VIEWED:") {
			m_list2->AppendString(episodeList[i]);
		}		
	}
}

cMain::~cMain() 
{

}

void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	m_list1->Clear();
	getEpisodes();
	string episodeToView = returnEpisode();

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
	recentEpisode = returnEpisode();

	// Get the strings of information to parse the directory and open the video file
	string seriesTitle = returnSeriesName();
	string seasonTitle = returnSeasonName();
	string episodeTitle = returnEpisodeName();

	openFile(seriesTitle, seasonTitle, episodeTitle);
	evt.Skip();
}
