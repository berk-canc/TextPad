#ifndef FRAME_H
#define FRAME_H
#include "wx/wxprec.h"
#include "wx/textfile.h"

class Frame : public wxFrame
{
public:
	Frame();
	~Frame();

private:
	wxString mTitle;
	wxMenuBar* m_pMenuBar;
	wxMenu* m_pFileMenu;
	wxMenu* m_pEditMenu;
	wxMenu* m_pAboutMenu;
	wxStatusBar* m_pStatusBar;
	wxTextCtrl* m_pTextCtrl;
	wxFont* m_pFont;
	wxTextFile mTextFile;
	bool mIsUntitled;
	bool mForceOpen;
	wxString mFilePathAndName;

	void New_Item_Clicked(wxCommandEvent& event);
	void Open_Item_Clicked(wxCommandEvent& event);
	void Save_Item_Clicked(wxCommandEvent& event);
	void Save_As_Item_Clicked(wxCommandEvent& event);
	void Exit_Item_Clicked(wxCommandEvent& event);
	void Copy_Item_Clicked(wxCommandEvent& event);
	void Cut_Item_Clicked(wxCommandEvent& event);
	void Paste_Item_Clicked(wxCommandEvent& event);
	void Options_Item_Clicked(wxCommandEvent& event);
	void About_Item_Clicked(wxCommandEvent& event);

	enum ComponentId
	{
		MENU_BAR, STATUS_BAR, TEXT_AREA, FILE_MENU, EDIT_MENU, 
		ABOUT_MENU, NEW_ITEM, OPEN_ITEM, SAVE_ITEM, SAVE_AS_ITEM, 
		EXIT_ITEM, COPY_ITEM, CUT_ITEM, PASTE_ITEM, OPTIONS_ITEM, ABOUT_ITEM
	};

	DECLARE_EVENT_TABLE()
};

#endif