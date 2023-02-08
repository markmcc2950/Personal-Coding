#pragma once
#include "wx/wx.h"
#include "string.h"
#include "RandomEpisode.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
	wxButton* m_btn1 = nullptr;
	wxTextCtrl* m_txt1 = nullptr;
	wxListBox* m_list1 = nullptr;
	wxListBox* m_list2 = nullptr;

	std::string recentEpisode = "";
	string episodeList[10];
	bool episodePicked = false;

	void OnButtonClicked(wxCommandEvent& evt);
	
	wxDECLARE_EVENT_TABLE();

private:
	
};

