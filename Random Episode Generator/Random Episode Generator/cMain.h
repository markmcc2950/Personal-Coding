#pragma once
#include "wx/wx.h"
#include <string>
#include "RandomEpisode.h"
#include "DirectoryHandler.h"

class cMain : public wxFrame {
public:
	cMain();
	~cMain();

public:
	void OnRandomButtonClicked(wxCommandEvent& evt);
	void onBrowseButtonClicked(wxCommandEvent& evt);
	void OnNextButtonClicked(wxCommandEvent& evt);
	void DestroyListBox(wxListBox* listBox);
	void DestroyButton(wxButton* button);
	
	wxDECLARE_EVENT_TABLE();

private:
	wxButton* m_btn1 = nullptr;
	wxButton* m_btn2 = nullptr;
	wxButton* m_btn3 = nullptr;
	wxTextCtrl* m_txt1 = nullptr;
	wxListBox* m_list1 = nullptr;
	wxListBox* m_list2 = nullptr;
	wxListBox* m_list3 = nullptr;

	std::string recentEpisode = "";
	std::string episodeList[10];
	bool episodePicked = false;
};

