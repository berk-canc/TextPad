#include "Frame.h"

BEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(NEW_ITEM, Frame::New_Item_Clicked)
	EVT_MENU(OPEN_ITEM, Frame::Open_Item_Clicked)
	EVT_MENU(SAVE_ITEM, Frame::Save_Item_Clicked)
	EVT_MENU(SAVE_AS_ITEM, Frame::Save_As_Item_Clicked)
	EVT_MENU(EXIT_ITEM, Frame::Exit_Item_Clicked)
	EVT_MENU(COPY_ITEM, Frame::Copy_Item_Clicked)
	EVT_MENU(CUT_ITEM, Frame::Cut_Item_Clicked)
	EVT_MENU(PASTE_ITEM, Frame::Paste_Item_Clicked)
	EVT_MENU(OPTIONS_ITEM, Frame::Options_Item_Clicked)
	EVT_MENU(ABOUT_ITEM, Frame::About_Item_Clicked)
	EVT_CLOSE(Frame::Exit_Item_Clicked)
END_EVENT_TABLE();

Frame::Frame() :
mTitle("Untitled : TexPad"), mIsUntitled(true), 
mFilePathAndName(""), mForceOpen(false), wxFrame((wxFrame*) NULL, -1, "Untitled : TextPad") 
{
	m_pMenuBar = new wxMenuBar();
	m_pFileMenu = new wxMenu(); 
	m_pEditMenu = new wxMenu();
	m_pAboutMenu = new wxMenu();
	m_pStatusBar = new wxStatusBar(this, STATUS_BAR);

	///////////////////////////////////////////////////
	m_pFileMenu->Append(NEW_ITEM, "New");
	m_pFileMenu->Append(OPEN_ITEM, "Open...");
	m_pFileMenu->Append(SAVE_ITEM, "Save");
	m_pFileMenu->Append(SAVE_AS_ITEM, "Save As...");
	m_pFileMenu->Append(EXIT_ITEM, "Exit");
	///////////////////////////////////////////////////
	m_pEditMenu->Append(COPY_ITEM, "Copy");
	m_pEditMenu->Append(CUT_ITEM, "Cut");
	m_pEditMenu->Append(PASTE_ITEM, "Paste");
	m_pEditMenu->Append(OPTIONS_ITEM, "Options");
	///////////////////////////////////////////////////
	m_pAboutMenu->Append(ABOUT_ITEM, "About TextPad");
	///////////////////////////////////////////////////

	m_pMenuBar->Append(m_pFileMenu, "File");
	m_pMenuBar->Append(m_pEditMenu, "Edit");
	m_pMenuBar->Append(m_pAboutMenu, "About");

	m_pFont = new wxFont(10, wxMODERN, wxNORMAL, wxNORMAL);
	m_pTextCtrl = new wxTextCtrl(this, TEXT_AREA, "", wxDefaultPosition, wxDefaultSize,
								 wxTE_MULTILINE, wxDefaultValidator, wxTextCtrlNameStr);

	m_pTextCtrl->SetFont(*m_pFont);

	this->SetMenuBar(m_pMenuBar);
	this->SetStatusBar(m_pStatusBar);
	this->Center();	
}


Frame::~Frame()
{
	delete m_pFont;
	mTextFile.Close();
}


void Frame::New_Item_Clicked(wxCommandEvent& event)
{

}


//CHECK THIS FUNCTION!
void Frame::Open_Item_Clicked(wxCommandEvent& event)
{
	//if the curent open document is good to close
	if( (m_pTextCtrl->IsModified() == false && mForceOpen == false) || mForceOpen == true)
	{
		wxFileDialog* openDialog = new wxFileDialog(this, "Open...", "", "", "*.txt", wxOPEN, wxDefaultPosition);
		int userInput = openDialog->ShowModal();

		if(userInput == wxID_OK)
		{
			//if file is opened successfuly (meaning Open returns true), mUntitled is false
			mIsUntitled = !( mTextFile.Open(openDialog->GetPath()) );
			
			mTextFile.Open(openDialog->GetPath());
			//A successful file open was done. So se file name and frame title.
			if(mIsUntitled == false)
			{
				mFilePathAndName = openDialog->GetPath();
				m_pTextCtrl->LoadFile(mFilePathAndName);			
				this->SetTitle(openDialog->GetFilename() + " : TextPad");
			}
			//file could not be saved. ERROR!
			else
			{
				wxMessageDialog* openErrorDialog = new wxMessageDialog(this, "Open Error: <005>",
					"TextPad", wxOK | wxICON_ERROR | wxCENTRE);
				
				openErrorDialog->Destroy();
			}

			mForceOpen = false;
		}

		if(userInput == wxCANCEL) { /* Just close the dialog. */ }

		openDialog->Destroy();
	}
	else
	{
		wxMessageDialog* saveDialog = new wxMessageDialog(this, "Do you want to save changes?", "TextPad", 
			wxYES_NO | wxCANCEL | wxNO_DEFAULT | wxICON_EXCLAMATION | wxCENTRE);
		
		int userInput = saveDialog->ShowModal();

		if(userInput == wxID_YES)
		{
			Save_Item_Clicked(event);
			Open_Item_Clicked(event);
		}

		if(userInput == wxID_NO)
		{
			mForceOpen = true;
			Open_Item_Clicked(event);
		}

		if(userInput == wxID_CANCEL) { /*Don't do nothing. Go back to the document.*/ }
		
		saveDialog->Destroy();
	}
}


void Frame::Save_Item_Clicked(wxCommandEvent& event)
{
	//Flag which determines whether or not the file can be 
	//saved by clicking 'File > Save' (w/o opening the dialog). 
	//This feature is refered as 'Quick Save.' 
	bool quietSave = true;

	//If it is a new file, open the dialog
	if(mIsUntitled == true)
	{
		wxFileDialog* saveDialog = new wxFileDialog(this, "Save As...", "", "*.txt", "Text files (*.txt)|*.txt", 
			wxSAVE | wxOVERWRITE_PROMPT, wxDefaultPosition);

		int userInput = saveDialog->ShowModal();

		if(userInput == wxID_OK)
		{
			//if file is saved successfuly (meaning SaveFile returns true), mUntitled is false
			mIsUntitled = !( m_pTextCtrl->SaveFile(saveDialog->GetPath()) );

			//A successful save was done. So se file name and frame title and block quiet save.
			if(mIsUntitled == false) 
			{
				mFilePathAndName = saveDialog->GetPath();
				this->SetTitle(saveDialog->GetFilename() + " : TextPad");
				//since file has been saved in dialog, don't continue quiet saving.
				quietSave = false;
			}
			//file could not be saved. ERROR!
			else
			{
				wxMessageDialog* saveErrorDialog = new wxMessageDialog(this, "Save Error: <001>",
					"TextPad", wxOK | wxICON_ERROR | wxCENTRE);
				//for safety, don't quiet save
				quietSave = false;

				saveErrorDialog->Destroy();
			}			
		}

		if(userInput == wxID_CANCEL) { /*Just close the saveDialog...*/ }

		saveDialog->Destroy();
	}


	//If the file already being edited, quick save it.
	if(quietSave == true)
	{
		m_pTextCtrl->SaveFile(mFilePathAndName);	
	}
}

//CHECK THIS FUNCTION!
void Frame::Save_As_Item_Clicked(wxCommandEvent& event)
{
	wxFileDialog* saveDialog = new wxFileDialog(this, "Save As...", "", "*.txt", "Text files (*.txt)|*.txt", 
		wxSAVE | wxOVERWRITE_PROMPT, wxDefaultPosition);

	int userInput = saveDialog->ShowModal();

	if(userInput == wxID_OK)
	{
		//if file is saved successfuly (meaning SaveFile returns true), mUntitled is false
		mIsUntitled = !( m_pTextCtrl->SaveFile(saveDialog->GetPath()) );

		//A successful save was done. So se file name and frame title and block quiet save.
		if(mIsUntitled == false) 
		{
			mFilePathAndName = saveDialog->GetPath();
			this->SetTitle(saveDialog->GetFilename() + " : TextPad");
		}
		//file could not be saved. ERROR!
		else
		{
			wxMessageDialog* saveErrorDialog = new wxMessageDialog(this, "Save Error: <004>",
				"TextPad", wxOK | wxICON_ERROR | wxCENTRE);

			saveErrorDialog->Destroy();
		}			
	}

	if(userInput == wxID_CANCEL) { /*Just close the saveDialog...*/ }

	saveDialog->Destroy();
}


//THIS EVENT IS BEING HANDLED WELL!/////////////////
void Frame::Exit_Item_Clicked(wxCommandEvent& event)
{
	//File is not modified. Good to close.
	if(m_pTextCtrl->IsModified() == false)
	{
		if(this->Destroy()) { /*closed application successfuly*/ }
		else
		{
			wxMessageDialog* closeErrorDialog = new wxMessageDialog(this, "Close Error: <002>",
				"TextPad", wxOK | wxICON_ERROR | wxCENTRE);

			closeErrorDialog->Destroy();
		}
	}
	//File is modified. Ask user what to do.
	else
	{
		wxMessageDialog* exitDialog = new wxMessageDialog(this, "Do you want to save changes?", "TextPad", 
			wxYES_NO | wxCANCEL | wxNO_DEFAULT | wxICON_EXCLAMATION | wxCENTRE);
		
		int userInput = exitDialog->ShowModal();

		if(userInput == wxID_YES)
		{
			Save_Item_Clicked(event);
		}

		if(userInput == wxID_NO)
		{
			if(this->Destroy()) { /*closed application successfuly*/ }
			else
			{
				wxMessageDialog* closeErrorDialog = new wxMessageDialog(this, "Close Error: <003>",
					"TextPad", wxOK | wxICON_ERROR | wxCENTRE);

				closeErrorDialog->Destroy();
			}
		}

		if(userInput == wxID_CANCEL) { /*Don't do nothing. Go back to the document.*/ }
		
		exitDialog->Destroy();
	}
}


void Frame::Copy_Item_Clicked(wxCommandEvent& event)
{
	m_pTextCtrl->Copy();
	//if empty, cant copy
}


void Frame::Cut_Item_Clicked(wxCommandEvent& event)
{
	m_pTextCtrl->Cut();
	//if empty, can't paste
}


void Frame::Paste_Item_Clicked(wxCommandEvent& event)
{
	m_pTextCtrl->Paste();
}

void Frame::Options_Item_Clicked(wxCommandEvent& event)
{
	wxLogMessage("Opts");
}


void Frame::About_Item_Clicked(wxCommandEvent& event)
{

}