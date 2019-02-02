﻿#include "TexGenMainFrame.h"
#include "LoggerGUI.h"
#include "Exporter.h"

#define EVT_BUTTON_MENU_RANGE(id1, id2, func) EVT_COMMAND_RANGE(id1, id2, wxEVT_COMMAND_BUTTON_CLICKED, func) EVT_COMMAND_RANGE(id1, id2, wxEVT_COMMAND_MENU_SELECTED, func)
#define EVT_CHECKBOX_MENU_RANGE(id1, id2, func) EVT_COMMAND_RANGE(id1, id2, wxEVT_COMMAND_CHECKBOX_CLICKED, func) EVT_COMMAND_RANGE(id1, id2, wxEVT_COMMAND_MENU_SELECTED, func)
#define EVT_RADIOBUTTON_MENU_RANGE(id1, id2, func) EVT_COMMAND_RANGE(id1, id2, wxEVT_COMMAND_RADIOBUTTON_SELECTED, func) EVT_COMMAND_RANGE(id1, id2, wxEVT_COMMAND_MENU_SELECTED, func)


BEGIN_EVENT_TABLE(CTexGenMainFrame, wxFrame)
EVT_MENU(ID_Quit, CTexGenMainFrame::OnQuit)
EVT_MENU(ID_About, CTexGenMainFrame::OnAbout)
EVT_MENU(ID_UserGuide, CTexGenMainFrame::OnUserGuide)
EVT_MENU(ID_Open, CTexGenMainFrame::OnOpen)
EVT_MENU(ID_Save, CTexGenMainFrame::OnSave)
EVT_MENU(ID_SaveScreenshot, CTexGenMainFrame::OnSaveScreenshot)
EVT_MENU(ID_OpenWiseTex, CTexGenMainFrame::OnOpenWiseTex)
EVT_MENU(ID_OpenTexGenv2, CTexGenMainFrame::OnOpenTexGenv2)
EVT_MENU(ID_OpenWeavePattern, CTexGenMainFrame::OnOpenWeavePattern)
EVT_MENU(ID_SaveGrid, CTexGenMainFrame::OnSaveGrid)
EVT_MENU(ID_SaveVoxel, CTexGenMainFrame::OnSaveVoxel)
EVT_MENU(ID_SaveVolumeMesh, CTexGenMainFrame::OnSaveVolumeMesh)
EVT_MENU(ID_SaveSurfaceMesh, CTexGenMainFrame::OnSaveSurfaceMesh)
EVT_MENU(ID_SaveIGES, CTexGenMainFrame::OnSaveIGES)

EVT_MENU(ID_SaveSTL, CTexGenMainFrame::OnSaveSTL)

EVT_MENU(ID_SaveSTEP, CTexGenMainFrame::OnSaveSTEP)
EVT_MENU(ID_SaveABAQUS, CTexGenMainFrame::OnSaveABAQUS)
EVT_MENU(ID_SaveABAQUSVoxels, CTexGenMainFrame::OnSaveABAQUSVoxels)
EVT_MENU(ID_ToggleControls, CTexGenMainFrame::OnWindow)
EVT_MENU(ID_ToggleLogWindow, CTexGenMainFrame::OnWindow)
EVT_MENU(ID_ToggleOutliner, CTexGenMainFrame::OnWindow)
EVT_MENU(ID_SaveTetgenMesh, CTexGenMainFrame::OnSaveTetgenMesh)

EVT_AUINOTEBOOK_PAGE_CHANGED(ID_LogNoteBook, CTexGenMainFrame::OnLogNotebook)
EVT_AUINOTEBOOK_PAGE_CHANGED(ID_ViewerNoteBook, CTexGenMainFrame::OnViewerNotebookPageChanged)
EVT_AUINOTEBOOK_PAGE_CLOSE(ID_ViewerNoteBook, CTexGenMainFrame::OnViewerNotebookClose)

EVT_CHECKBOX_MENU_RANGE(ID_RenderNodes, ID_TrimtoDomain, CTexGenMainFrame::OnRendering)
EVT_BUTTON_MENU_RANGE(ID_ChangeBackgroundColor, ID_ChangeSurfaceColor, CTexGenMainFrame::OnRendering)
EVT_BUTTON_MENU_RANGE(ID_CreateEmptyTextile, ID_MaxNestLayers, CTexGenMainFrame::OnTextiles)
EVT_BUTTON_MENU_RANGE(ID_CreateYarn, ID_YarnFibreVolumeFraction, CTexGenMainFrame::OnModeller)
EVT_RADIOBUTTON_MENU_RANGE(ID_SelectTool, ID_ScaleTool, CTexGenMainFrame::OnModeller)
EVT_CHECKBOX_MENU_RANGE(ID_FilterNodes, ID_Relative, CTexGenMainFrame::OnModeller)
EVT_BUTTON_MENU_RANGE(ID_CreateDomainPlanes, ID_DeleteDomain, CTexGenMainFrame::OnDomains)
EVT_BUTTON_MENU_RANGE(ID_RunScript, ID_RecordMacro, CTexGenMainFrame::OnPython)

//Options
EVT_MENU(ID_OutputMessages, CTexGenMainFrame::OnOutputMessages)
EVT_MENU(ID_PatternDraft, CTexGenMainFrame::OnPatternDraft)
EVT_MENU(ID_DomainVolumeFraction, CTexGenMainFrame::OnDomainVolumeFraction)
EVT_MENU(ID_QuickDomainVolumeFraction, CTexGenMainFrame::OnQuickDomainVolumeFraction)
//



EVT_TEXT_ENTER(ID_PositionX, CTexGenMainFrame::OnPosition)
EVT_TEXT_ENTER(ID_PositionY, CTexGenMainFrame::OnPosition)
EVT_TEXT_ENTER(ID_PositionZ, CTexGenMainFrame::OnPosition)
EVT_TEXT_ENTER(ID_SnapSize, CTexGenMainFrame::OnSnapSize)

EVT_TEXT(ID_PositionX, CTexGenMainFrame::OnPosition)
EVT_TEXT(ID_PositionY, CTexGenMainFrame::OnPosition)
EVT_TEXT(ID_PositionZ, CTexGenMainFrame::OnPosition)
EVT_TEXT(ID_SnapSize, CTexGenMainFrame::OnSnapSize)

EVT_AUI_PANE_BUTTON(CTexGenMainFrame::OnPaneButton)

END_EVENT_TABLE()

BEGIN_EVENT_TABLE(CNoteBookHandler, wxEvtHandler)
EVT_SET_FOCUS(CNoteBookHandler::OnSetFocus)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(CAbaqusVoxelInput, wxDialog)
EVT_UPDATE_UI(XRCID("Offset"), CAbaqusVoxelInput::OnOffsetUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(CVolumeMeshOptions, wxDialog)
EVT_UPDATE_UI(XRCID("PeriodicBoundaries"), CVolumeMeshOptions::OnPeriodicBoundariesUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(CSurveyDialog, wxDialog)
EVT_BUTTON(XRCID("NextTime"), CSurveyDialog::OnClickNextTime)
EVT_BUTTON(XRCID("NotAgain"), CSurveyDialog::OnClickNotAgain)
EVT_HYPERLINK(XRCID("TakeSurvey"), CSurveyDialog::OnClickTakeSurvey)
END_EVENT_TABLE()
;


CTexGenMainFrame::CTexGenMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
, m_pViewerNotebook(NULL)
, m_pLogNotebook(NULL)
//, m_pPythonConsole(NULL)
, m_pPythonOutput(NULL)
, m_pTexGenOutput(NULL)
, m_pControls(NULL)
, m_pOutliner(NULL)
{
	m_Manager.SetManagedWindow(this);

	wxMenu *pMenuFile = new wxMenu;

	pMenuFile->Append(ID_Open, wxT("&Open..."));
	pMenuFile->AppendSeparator();
	pMenuFile->Append(ID_Save, wxT("&Save as..."));
	pMenuFile->AppendSeparator();
	{
		wxMenu *pImportSubMenu = new wxMenu;
		pImportSubMenu->Append(ID_OpenTexGenv2, wxT("&TexGen v2 File..."));
		pImportSubMenu->Append(ID_OpenWiseTex, wxT("&WiseTex File..."));
		pImportSubMenu->Append(ID_OpenWeavePattern, wxT("&Weave Pattern File..."));
		pMenuFile->Append(wxID_ANY, wxT("&Import"), pImportSubMenu);
	}
	{
		wxMenu *pExportSubMenu = new wxMenu;
		pExportSubMenu->Append(ID_SaveScreenshot, wxT("&Image (*.png)"));
		pExportSubMenu->Append(ID_SaveGrid, wxT("&Grid (*.grd)"));
		pExportSubMenu->Append(ID_SaveVoxel, wxT("&Voxel (*.vox)"));
		pExportSubMenu->Append(ID_SaveIGES, wxT("&IGES (*.igs)"));
		pExportSubMenu->Append(ID_SaveSTEP, wxT("&STEP (*.stp)"));
		pExportSubMenu->Append(ID_SaveSTL, wxT("&STL (*.stl)"));


		pExportSubMenu->Append(ID_SaveSurfaceMesh, wxT("Sur&face Mesh..."));
		
		

		{
			wxMenu *pAbaqusSubMenu = new wxMenu;
			pAbaqusSubMenu->Append(ID_SaveVolumeMesh, wxT("&Volume Mesh..."));
			pAbaqusSubMenu->Append(ID_SaveTetgenMesh, wxT("&TetgenMesh..."));
			pAbaqusSubMenu->Append(ID_SaveABAQUS, wxT("&Dry Fibre File..."));
			pAbaqusSubMenu->Append(ID_SaveABAQUSVoxels, wxT("&Voxel File..."));
			pExportSubMenu->Append(wxID_ANY, wxT("&ABAQUS (*.inp)"), pAbaqusSubMenu);
		}
		pMenuFile->Append(wxID_ANY, wxT("&Export..."), pExportSubMenu);
	}
	pMenuFile->AppendSeparator();
	pMenuFile->Append(ID_Quit, wxT("&Exit"));

	wxMenu *pMenuWindow = new wxMenu;
	pMenuWindow->Append(ID_ToggleControls, wxT("&Controls"), wxT(""), wxITEM_CHECK);
	pMenuWindow->Append(ID_ToggleLogWindow, wxT("&Logs"), wxT(""), wxITEM_CHECK);
	pMenuWindow->Append(ID_ToggleOutliner, wxT("&Outliner"), wxT(""), wxITEM_CHECK);


	wxMenuBar *pMenuBar = new wxMenuBar;
	pMenuBar->Append(pMenuFile, wxT("&File"));
	pMenuBar->Append(pMenuWindow, wxT("&Window"));





	// The icon is defined in the resource file
	SetIcon(wxICON(TexGen));

/*	CreateStatusBar();
	SetStatusText(wxT("Welcome to TexGen!"));
	SetStatusBarPane(-1);*/

	wxFont ConsoleFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

	// This is our main window
	m_pViewerNotebook = new wxAuiNotebook(this, ID_ViewerNoteBook, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TOP | wxAUI_NB_TAB_MOVE | wxAUI_NB_CLOSE_ON_ALL_TABS);

	m_pLogNotebook = new wxAuiNotebook(this, ID_LogNoteBook, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS);
	{
/*		wxSize ImageSize(16, 16);

		wxImageList *pImageList = new wxImageList(ImageSize.GetWidth(), ImageSize.GetHeight());

		// should be able to add them as icons in which case they will automatically be transparent
		// doesn't seem to work however!! Does work in the wxnotebook and arttest sample, don't know why...
		pImageList->Add(wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, ImageSize));
		pImageList->Add(wxArtProvider::GetIcon(wxART_WARNING, wxART_OTHER, ImageSize));
		pImageList->Add(wxArtProvider::GetIcon(wxART_ERROR, wxART_OTHER, ImageSize));

		m_pLogNotebook->AssignImageList(pImageList);*/

		/*m_pPythonConsole = new CPythonConsole(m_PythonWrapper, m_pLogNotebook, wxID_ANY, wxT(">>> "),
			wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_DONTWRAP);
		m_pPythonConsole->SetFont(ConsoleFont);//*/

		m_pPythonOutput = new wxTextCtrl(m_pLogNotebook, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_DONTWRAP | wxTE_RICH);
		m_pPythonOutput->SetFont(ConsoleFont);

		m_pTexGenOutput = new wxTextCtrl(m_pLogNotebook, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_DONTWRAP | wxTE_RICH);
		m_pTexGenOutput->SetFont(ConsoleFont);

		//m_pLogNotebook->AddPage(m_pPythonConsole, wxT("Python Console"), true, wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_OTHER, wxSize(16, 16)));
		m_pLogNotebook->AddPage(m_pPythonOutput, wxT("Python Output"), false, wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_OTHER, wxSize(16, 16)));
		m_pLogNotebook->AddPage(m_pTexGenOutput, wxT("Log Output"), false, wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_OTHER, wxSize(16, 16)));
	}

	// This is the control window where all the GUI controls go (buttons, etc...)
/*	wxSashLayoutWindow *pControlSashWindow = new wxSashLayoutWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN);
	pControlSashWindow->SetOrientation(wxLAYOUT_VERTICAL);
	pControlSashWindow->SetAlignment(wxLAYOUT_LEFT);*/

	m_pControls = new CControlsWindow(pMenuBar, this, ID_ControlsWindow);
//	pControlSashWindow->SetDefaultSize(m_pControls->GetSize());

/*	wxLayoutAlgorithm layout;
	layout.LayoutFrame(this, m_pViewerNotebook);

	SetSizeHints(200, 200);*/

	m_pViewerNotebook->PushEventHandler(new CNoteBookHandler(*this));
	m_Manager.AddPane(m_pViewerNotebook, wxAuiPaneInfo().Name(wxT("Viewer")).CenterPane().PaneBorder(false));
	m_Manager.AddPane(m_pLogNotebook, wxAuiPaneInfo().Name(wxT("Logs")).Bottom().BestSize(wxSize(300, 300)).Caption(wxT("Log windows")));
	m_Manager.AddPane(m_pControls, wxAuiPaneInfo().Name(wxT("Controls")).Left().TopDockable(false).BottomDockable(false).Caption(wxT("Controls")));

	m_pOutliner = new wxPanel();
	if (wxXmlResource::Get()->LoadPanel(m_pOutliner, this, wxT("Outliner")))
	{
		m_pOutliner->PushEventHandler(new COutlineHandler(*this));
//		m_pOutliner->SetSize(200, 350);
//		UpdateOutlinerItems();
	}

	m_Manager.AddPane(m_pOutliner, wxAuiPaneInfo().Name(wxT("Outliner")).Right().Hide().TopDockable(false).BottomDockable(false).Caption(wxT("Outliner")));

	m_Manager.Update();

	bool bSuccess = m_Manager.LoadPerspective(wxConfigBase::Get()->Read(wxT("Perspective"), wxEmptyString));

	wxMenu *pMenuHelp = new wxMenu;
	pMenuHelp->Append(ID_About, wxT("&About..."));
	pMenuHelp->Append(ID_UserGuide, wxT("&User Guide"));
	pMenuBar->Append(pMenuHelp, wxT("&Help"));
	SetMenuBar(pMenuBar);

	UpdateWindowChecks();

	SetDropTarget(new CFileDropTarget());
}

CTexGenMainFrame::~CTexGenMainFrame(void)
{
	while (m_pViewerNotebook->GetPageCount() != 0)
	{
		m_pViewerNotebook->RemovePage(0);
	}
	m_pViewerNotebook->PopEventHandler(true);
	map<string, CTexGenRenderWindow*>::iterator itRenderWindow;
	for (itRenderWindow = m_RenderWindows.begin(); itRenderWindow != m_RenderWindows.end(); ++itRenderWindow)
	{
		itRenderWindow->second->CleanUp();
		itRenderWindow->second->Delete();
	}
	m_RenderWindows.clear();

	wxConfigBase::Get()->Write(wxT("Perspective"), m_Manager.SavePerspective());

	m_pOutliner->PopEventHandler( true );

	m_Manager.UnInit();
}

void CTexGenMainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(TRUE);
}

void CTexGenMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	string Title("About");
	stringstream StringStream;
	StringStream << "Saturn PAKM 3D" << endl;
	StringStream << "Version " << CTexGen::Instance().GetVersion() << " (Official Build) (32-bit)" << endl;
	StringStream << "Saturn PAKM 3D is made possible by the TexGen open source project and other open source software." << endl;
	StringStream << "This is free software licensed under the GNU GPLv3." << endl;
	StringStream << "Copyright " << __DATE__ << " PNRPU All rights reserved." << endl;
	wxMessageBox(ConvertString(StringStream.str()), ConvertString(Title), wxOK | wxICON_INFORMATION, this);
}

void CTexGenMainFrame::OnUserGuide(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDialog = new wxDialog( NULL, wxID_ANY, wxT("User Guide"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);

	wxBoxSizer *BoxSizer = new wxBoxSizer (wxVERTICAL);
	wxSizerFlags SizerFlags(0);
	SizerFlags.Border(wxALL,10);
	SizerFlags.Expand();

	BoxSizer->Add(new wxStaticText( pDialog, wxID_ANY, wxT("The TexGen User Guide can be found here:") ), SizerFlags );
	BoxSizer->Add( new wxHyperlinkCtrl( pDialog, -1, wxT("User Guide"), wxT("http://texgen.sourceforge.net/index.php/User_Guide")), SizerFlags );
	pDialog->SetSizer( BoxSizer );
	BoxSizer->Fit( pDialog );

	pDialog->ShowModal();
	pDialog->Destroy();
}

void CTexGenMainFrame::ReceiveOutput(string Text, OUTPUT_TYPE OutputType, bool bError, bool bInteractive)
{
	wxTextAttr PrevStyle;
	int iPage;
	wxTextCtrl *pOutputWindow;
	if (OutputType == OUTPUT_PYTHON)
	{
		/*if (bInteractive)
		{
			iPage = 0;
			//pOutputWindow = m_pPythonConsole;
		}
		else//*/
		{
			iPage = 1;
			pOutputWindow = m_pPythonOutput;
		}
	}
	else if (OutputType == OUTPUT_TEXGEN)
	{
		iPage = 2;
		pOutputWindow = m_pTexGenOutput;
	}
	else
		return;

	PrevStyle = pOutputWindow->GetDefaultStyle();
	if (bError)
		pOutputWindow->SetDefaultStyle(wxTextAttr(*wxRED));
	pOutputWindow->AppendText(ConvertString(Text));
	pOutputWindow->SetDefaultStyle(PrevStyle);
	if (m_pLogNotebook->GetSelection() != iPage)
	{
		if (bError)
			m_pLogNotebook->SetPageBitmap(iPage, wxArtProvider::GetBitmap(wxART_ERROR, wxART_OTHER, wxSize(16, 16)));
		else/* if (m_pLogNotebook->GetPageBitmap(iPage) != 2)*/
			m_pLogNotebook->SetPageBitmap(iPage, wxArtProvider::GetBitmap(wxART_WARNING, wxART_OTHER, wxSize(16, 16)));
	}
	// Will update the frame, this is usefull for operations which take a long time
	// to complete in python. Each time a message is sent from python the window
	// will be updated so that it avoids the user thinking the program has crashed.
//	Update();		// Doesn't seem to work...
	wxSafeYield();
}

void CTexGenMainFrame::OnOpen(wxCommandEvent& event)
{
	TGLOG("CTexGenMainFrame::OnOpen");
	wxFileDialog dialog
	(
		this,
		wxT("Open TexGen file"),
		wxGetCwd(),
		wxEmptyString,
		wxT("TexGen XML file (*.tg3)|*.tg3"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR
	);
	dialog.CentreOnParent();
	if (dialog.ShowModal() == wxID_OK)
	{
		auto path = ConvertString(dialog.GetPath());
		CTexGen::Instance().ReadFromXML(path);
	}
}

void CTexGenMainFrame::OnSave(wxCommandEvent& event)
{
	TGLOG("CTexGenMainFrame::OnSave");
	wxArrayString Options;
	Options.Add(wxT("Minimal (Textile data only)"));
	Options.Add(wxT("Standard (Textile and yarn data)"));
	Options.Add(wxT("Full (Textile, yarn and mesh data)"));
	wxSingleChoiceDialog SaveTypeDialog(this, wxT("Please select how much data to save."), wxT("Save dialog"), Options);
	SaveTypeDialog.SetSelection(OUTPUT_STANDARD);
	if (SaveTypeDialog.ShowModal() == wxID_OK)
	{
		wxFileDialog dialog
		(
			this,
			wxT("Save TexGen file"),
			wxGetCwd(),
			wxEmptyString,
			wxT("TexGen XML file (*.tg3)|*.tg3"),
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
		);
		dialog.CentreOnParent();
		if (dialog.ShowModal() == wxID_OK)
		{
			auto path = ConvertString(dialog.GetPath());
			switch (SaveTypeDialog.GetSelection())
			{
			case OUTPUT_MINIMAL:
				CTexGen::Instance().SaveToXML(path, GetTextileSelection(), OUTPUT_MINIMAL); break;
			case OUTPUT_STANDARD:
				CTexGen::Instance().SaveToXML(path, GetTextileSelection(), OUTPUT_STANDARD); break;
			case OUTPUT_FULL:
				CTexGen::Instance().SaveToXML(path, GetTextileSelection(), OUTPUT_FULL); break;
			}
		}
	}
}

void CTexGenMainFrame::OnSaveScreenshot(wxCommandEvent& event)
{
	wxDialog ScreenShot;
	if (wxXmlResource::Get()->LoadDialog(&ScreenShot, this, wxT("ScreenShot")))
	{
		if ( ScreenShot.ShowModal() == wxID_OK )
		{
			wxFileDialog dialog
			(
				this,
				wxT("Save Screenshot"),
				wxGetCwd(),
				wxEmptyString,
				wxT("PNG image (*.png)|*.png"),
				wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
			);
			dialog.CentreOnParent();
			if (dialog.ShowModal() == wxID_OK)
			{
				string Command;
				Command = "GetRenderWindow().TakeScreenShot(r\"";
				Command += ConvertString(dialog.GetPath());
				Command += "\", ";
				
				wxSpinCtrl* MagCtrl = (wxSpinCtrl*)FindWindow(XRCID("Magnification"));
				
				Command += stringify(MagCtrl->GetValue());
				Command += ")";
				SendPythonCode(Command);
			}
		}
	}
}

void CTexGenMainFrame::OnOpenWiseTex(wxCommandEvent& event)
{
	wxFileDialog dialog
	(
		this,
		wxT("Open WiseTex file"),
		wxGetCwd(),
		wxEmptyString,
		wxT("WiseTex FlexComp file (*.cfl)|*.cfl"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR
	);
	dialog.CentreOnParent();
	if (dialog.ShowModal() == wxID_OK)
	{
		string Command;
		Command = "from TexGen.WiseTex import *\n";
		Command += "ImportWiseTex(r\"";
		Command += ConvertString(dialog.GetPath());
		Command += "\")";
		SendPythonCode(Command);
	}
}

void CTexGenMainFrame::OnOpenTexGenv2(wxCommandEvent& event)
{
	wxFileDialog dialog
	(
		this,
		wxT("Open TexGen v2 file"),
		wxGetCwd(),
		wxEmptyString,
		wxT("TexGen v2 path file (*.pth)|*.pth"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR
	);
	dialog.CentreOnParent();
	if (dialog.ShowModal() == wxID_OK)
	{
		string Command;
		Command = "from TexGen.TexGenv2 import *\n";
		Command += "ImportTexGenv2(r\"";
		Command += ConvertString(dialog.GetPath());
		Command += "\")";
		SendPythonCode(Command);
	}
}

void CTexGenMainFrame::OnOpenWeavePattern(wxCommandEvent& event)
{
	wxFileDialog dialog
	(
		this,
		wxT("Open Weave Pattern file"),
		wxGetCwd(),
		wxEmptyString,
		wxT("Weave Pattern file (*.txt)|*.txt"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR
	);
	dialog.CentreOnParent();
	if (dialog.ShowModal() == wxID_OK)
	{
		string Command;
		Command = "from TexGen.WeavePattern import *\n";
		Command += "ImportWeavePattern(r\"";
		Command += ConvertString(dialog.GetPath());
		Command += "\")";
		SendPythonCode(Command);
	}
}

void CTexGenMainFrame::OnSaveGrid(wxCommandEvent& event)
{
	wxFileDialog dialog
	(
		this,
		wxT("Save Grid file"),
		wxGetCwd(),
		wxEmptyString,
		wxT("Grid file (*.grd)|*.grd"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
	);
	dialog.CentreOnParent();
	string TextileName = GetTextileSelection();


	wxString X, Y;
	wxDialog GridResolution;
	if (wxXmlResource::Get()->LoadDialog(&GridResolution, this, wxT("GridResolution")))
	{
		XRCCTRL(GridResolution, "X", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &X));
		XRCCTRL(GridResolution, "Y", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &Y));
		XRCCTRL(GridResolution, "Z", wxTextCtrl)->Disable();

		if (GridResolution.ShowModal() == wxID_OK)
		{
			long iX, iY;
			X.ToLong(&iX);
			Y.ToLong(&iY);

			if (dialog.ShowModal() == wxID_OK)
			{
				string Command;
				Command = "from TexGen.GridFile import *\n";
				Command += "ExportGridFile(r\"";
				Command += ConvertString(dialog.GetPath());
				Command += "\", \"";
				Command += TextileName;
				Command += "\", (";
				Command += stringify(iX) + ", ";
				Command += stringify(iY);
				Command += "))";
				SendPythonCode(Command);
			}
		}
	}
}

void CTexGenMainFrame::OnSaveVoxel(wxCommandEvent& event)
{
	wxFileDialog dialog
	(
		this,
		wxT("Save Voxel file"),
		wxGetCwd(),
		wxEmptyString,
		wxT("Voxel file (*.vox)|*.vox"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
	);
	dialog.CentreOnParent();
	string TextileName = GetTextileSelection();

	wxString X, Y, Z;
	wxDialog GridResolution;
	if (wxXmlResource::Get()->LoadDialog(&GridResolution, this, wxT("GridResolution")))
	{
		XRCCTRL(GridResolution, "X", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &X));
		XRCCTRL(GridResolution, "Y", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &Y));
		XRCCTRL(GridResolution, "Z", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &Z));

		if (GridResolution.ShowModal() == wxID_OK)
		{
			long iX, iY, iZ;
			X.ToLong(&iX);
			Y.ToLong(&iY);
			Z.ToLong(&iZ);

			if (dialog.ShowModal() == wxID_OK)
			{
				string Command;
				Command = "from TexGen.FlowTex import *\n";
				Command += "ExportFlowTex(r\"";
				Command += ConvertString(dialog.GetPath());
				Command += "\", \"";
				Command += TextileName;
				Command += "\", (";
				Command += stringify(iX) + ", ";
				Command += stringify(iY) + ", ";
				Command += stringify(iZ);
				Command += "))";
				SendPythonCode(Command);
			}
		}
	}
}

void CTexGenMainFrame::OnSaveVolumeMesh(wxCommandEvent& event)
{
	string TextileName = GetTextileSelection();
	CTextile* pTextile = TEXGEN.GetTextile(TextileName);

	double dSeedSize = 1;

	if ( pTextile )
	{
		const CDomain* pDomain = pTextile->GetDomain();
		if ( pDomain )
		{
			pair<XYZ,XYZ> DomainSize = pDomain->GetMesh().GetAABB();
			dSeedSize = GetLength(DomainSize.first, DomainSize.second)/20.0;
		}
	}
	int iElementOrder = 0;
	bool bProjectMidSideNodes = true;
	bool bPeriodic = false;
	int iBoundaryConditions;

	wxString SeedSize = wxString::Format(wxT("%.4f"), dSeedSize);
	
	wxString MergeTolerance = wxT("0.001");
	CVolumeMeshOptions MeshOptions(this);
	
	{
		XRCCTRL(MeshOptions, "ElementOrder", wxRadioBox)->SetValidator(wxGenericValidator(&iElementOrder));
		XRCCTRL(MeshOptions, "ProjectMidSideNodes", wxCheckBox)->SetValidator(wxGenericValidator(&bProjectMidSideNodes));
		XRCCTRL(MeshOptions, "Periodic", wxCheckBox)->SetValidator(wxGenericValidator(&bPeriodic));
		XRCCTRL(MeshOptions, "SeedSize", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &SeedSize));
		XRCCTRL(MeshOptions, "MergeTolerance", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &MergeTolerance));
		XRCCTRL(MeshOptions, "PeriodicBoundaries", wxRadioBox)->SetValidator(wxGenericValidator(&iBoundaryConditions));

		wxRadioBox* PeriodicBoundariesCtrl = (wxRadioBox*)FindWindow(XRCID("PeriodicBoundaries"));
		PeriodicBoundariesCtrl->Enable((unsigned int)STAGGERED_BC, false);

		if (MeshOptions.ShowModal() == wxID_OK)
		{
			wxFileDialog dialog
			(
				this,
				wxT("Save Mesh file"),
				wxGetCwd(),
				wxEmptyString,
				wxT("ABAQUS input file (*.inp)|*.inp|")
				wxT("VTK unstructured grid file (*.vtu)|*.vtu"),
				wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
			);
			dialog.CentreOnParent();
			if (dialog.ShowModal() == wxID_OK)
			{
				stringstream Command;
				if ( bPeriodic )
					Command << "mesher = CMesher(" << iBoundaryConditions << ")" << endl;
				else
					Command << "mesher = CMesher()" << endl;
				if (iElementOrder == 1)
					Command << "mesher.SetQuadratic(True)" << endl;
				if (!bProjectMidSideNodes)
					Command << "mesher.SetProjectMidSideNodes(False)" << endl;
				if (bPeriodic)
					Command << "mesher.SetPeriodic(True)" << endl;
				if (!SeedSize.IsEmpty())
					Command << "mesher.SetSeed(" << ConvertString(SeedSize) << ")" << endl;
				if (!MergeTolerance.IsEmpty())
					Command << "mesher.SetMergeTolerance(" << ConvertString(MergeTolerance) << ")" << endl;
				Command << "mesher.CreateMesh(r'" << TextileName << "')" << endl;
				switch (dialog.GetFilterIndex())
				{
				case 0:
					Command << "mesher.SaveVolumeMeshToABAQUS(r'" << ConvertString(dialog.GetPath()) << "', '" << TextileName << "')" << endl;
					break;
				case 1:
					Command << "mesher.SaveVolumeMeshToVTK(r'" << ConvertString(dialog.GetPath()) << "')" << endl;
					break;
				}
				SendPythonCode(Command.str());
			}
		}
	}
}

void CTexGenMainFrame::OnSaveSTL(wxCommandEvent& event) {
	string TextileName = GetTextileSelection();
	bool bExportDomain = false;
	bool bTrimSurface = true;
	wxDialog MeshOptions;
	if (wxXmlResource::Get()->LoadDialog(&MeshOptions, this, wxT("SurfaceMeshOptions")))
	{
		XRCCTRL(MeshOptions, "TrimSurface", wxCheckBox)->SetValidator(wxGenericValidator(&bTrimSurface));
		XRCCTRL(MeshOptions, "ExportDomain", wxCheckBox)->SetValidator(wxGenericValidator(&bExportDomain));

		if (MeshOptions.ShowModal() == wxID_OK)
		{
			wxFileDialog dialog
			(
				this,
				wxT("Save STL file"),
				wxGetCwd(),
				wxEmptyString,
				wxT("ASCII STL file (*.stl)|*.stl|")
				wxT("Binary STL file (*.stl;*stlb)|*.stl;*stlb|"),
				wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
			);
			dialog.CentreOnParent();
			if (dialog.ShowModal() == wxID_OK) {
				stringstream Command;
				Command << "mesh = CMesh()" << endl;
				Command << "textile = GetTextile(r'" << TextileName << "')" << endl;
				if (bTrimSurface) {
					Command << "textile.AddSurfaceToMesh(mesh, True)" << endl;
				}
				else {
					Command << "textile.AddSurfaceToMesh(mesh, False)" << endl;
				}
				if (bExportDomain) {
					Command << "mesh.InsertMesh(textile.GetDomain().GetMesh())" << endl;
				}
				if (dialog.GetFilterIndex() == 0) {
					Command << "mesh.SaveToSTLAscii(r'" << ConvertString(dialog.GetPath()) << "')" << endl;
				}
				else {
					Command << "mesh.SaveToSTLBinary(r'" << ConvertString(dialog.GetPath()) << "')" << endl;
				}
				SendPythonCode(Command.str());
			}
		}
	}


}

void CTexGenMainFrame::OnSaveSurfaceMesh(wxCommandEvent& event)
{
	string TextileName = GetTextileSelection();

	bool bExportDomain = false;
	bool bTrimSurface = true;
	wxDialog MeshOptions;
	if (wxXmlResource::Get()->LoadDialog(&MeshOptions, this, wxT("SurfaceMeshOptions")))
	{
		XRCCTRL(MeshOptions, "TrimSurface", wxCheckBox)->SetValidator(wxGenericValidator(&bTrimSurface));
		XRCCTRL(MeshOptions, "ExportDomain", wxCheckBox)->SetValidator(wxGenericValidator(&bExportDomain));

		if (MeshOptions.ShowModal() == wxID_OK)
		{
			wxFileDialog dialog
			(
				this,
				wxT("Save Mesh file"),
				wxGetCwd(),
				wxEmptyString,
				wxT("VTK unstructured grid file (*.vtu)|*.vtu|")
				wxT("SCIRun file (*.pts)|*.pts|"),
				wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
			);
			dialog.CentreOnParent();
			if (dialog.ShowModal() == wxID_OK)
			{
				stringstream Command;
				Command << "mesh = CMesh()" << endl;
				Command << "textile = GetTextile(r'" << TextileName << "')" << endl;
				if (bTrimSurface)
					Command << "textile.AddSurfaceToMesh(mesh, True)" << endl;
				else
					Command << "textile.AddSurfaceToMesh(mesh, False)" << endl;
				if (bExportDomain)
					Command << "mesh.InsertMesh(textile.GetDomain().GetMesh())" << endl;
				switch (dialog.GetFilterIndex())
				{
				case 0:
					Command << "mesh.SaveToVTK(r'" << ConvertString(dialog.GetPath()) << "')" << endl;
					break;
				case 1:
					Command << "mesh.SaveToSCIRun(r'" << ConvertString(dialog.GetPath()) << "')" << endl;
					break;
				}
				SendPythonCode(Command.str());
			}
		}
	}
}

void CTexGenMainFrame::OnSaveIGES(wxCommandEvent& event)
{
	bool bExportDomain = true;
	bool bSubtractYarns = false;
	wxDialog ExportOptions;
	if (wxXmlResource::Get()->LoadDialog(&ExportOptions, this, wxT("ExportOptions")))
	{
		XRCCTRL(ExportOptions, "ExportDomain", wxCheckBox)->SetValidator(wxGenericValidator(&bExportDomain));
		XRCCTRL(ExportOptions, "SubtractYarns", wxCheckBox)->SetValidator(wxGenericValidator(&bSubtractYarns));
		if (ExportOptions.ShowModal() == wxID_OK)
		{
			wxFileDialog dialog
			(
				this,
				wxT("Save IGES file"),
				wxGetCwd(),
				wxEmptyString,
				wxT("IGES file (*.igs;*.iges)|*.igs;*.iges"),
				wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
			);
			dialog.CentreOnParent();
			if (dialog.ShowModal() == wxID_OK)
			{
				auto Exporter = new CExporter();
				Exporter->SetExportDomain(bExportDomain);
				Exporter->SetSubtractYarns(bSubtractYarns);
				auto path = ConvertString(dialog.GetPath());
				auto textileName = GetTextileSelection();
				Exporter->OutputTextileToIGES(path, textileName);
			}
		}
	}
}

void CTexGenMainFrame::OnSaveSTEP(wxCommandEvent& event)
{
	bool bExportDomain = true;
	bool bSubtractYarns = false;
	wxDialog ExportOptions;
	if (wxXmlResource::Get()->LoadDialog(&ExportOptions, this, wxT("ExportOptions")))
	{
		XRCCTRL(ExportOptions, "ExportDomain", wxCheckBox)->SetValidator(wxGenericValidator(&bExportDomain));
		XRCCTRL(ExportOptions, "SubtractYarns", wxCheckBox)->SetValidator(wxGenericValidator(&bSubtractYarns));
		if (ExportOptions.ShowModal() == wxID_OK) {
			wxFileDialog dialog
			(
				this,
				wxT("Save STEP file"),
				wxGetCwd(),
				wxEmptyString,
				wxT("STEP file (*.stp;*.step)|*.stp;*.step"),
				wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
			);
			dialog.CentreOnParent();
			if (dialog.ShowModal() == wxID_OK)
			{
				auto Exporter = new CExporter();
				Exporter->SetExportDomain(bExportDomain);
				Exporter->SetSubtractYarns(bSubtractYarns);
				auto path = ConvertString(dialog.GetPath());
				auto textileName = GetTextileSelection();
				Exporter->OutputTextileToSTEP(path, textileName);
			}
		}
	}
}

void CTexGenMainFrame::OnSaveABAQUS(wxCommandEvent& event)
{

	string TextileName = GetTextileSelection();
	stringstream Command;

	wxString XScale = wxT("1.0");
	wxString YScale = wxT("1.0");
	wxString ZScale = wxT("1.0");
	//wxString YoungsModulus = wxT("13000.0");
	//wxString PoissonsRatio = wxT("0.3");
	wxString IntersectionTolerance = wxT("0.0000001");
	bool bAdjustIntersections = false;
	bool bIncludePlates = false;
	bool bRegenerateMesh = false;
	int iElementType = 0;
	
	wxFileDialog dialog
	(
		this,
		wxT("Save Abaqus file"),
		wxGetCwd(),
		wxEmptyString,
		wxT("Abaqus input file (*.inp)|*.inp"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
	);
	dialog.CentreOnParent();

	wxDialog AbaqusInput;
	if (wxXmlResource::Get()->LoadDialog(&AbaqusInput, this, wxT("AbaqusOptions")))
	{
		XRCCTRL(AbaqusInput, "XScale", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &XScale));
		XRCCTRL(AbaqusInput, "YScale", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &YScale));
		XRCCTRL(AbaqusInput, "ZScale", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &ZScale));
		/*XRCCTRL(AbaqusInput, "YoungsModulus", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &YoungsModulus));
		XRCCTRL(AbaqusInput, "PoissonsRatio", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &PoissonsRatio));*/
		XRCCTRL(AbaqusInput, "AdjustIntersections", wxCheckBox)->SetValidator(wxGenericValidator(&bAdjustIntersections));
		XRCCTRL(AbaqusInput, "IntersectionTolerance", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &IntersectionTolerance));
		XRCCTRL(AbaqusInput, "IncludePlates", wxCheckBox)->SetValidator(wxGenericValidator(&bIncludePlates));
		XRCCTRL(AbaqusInput, "RegenerateMesh", wxCheckBox)->SetValidator(wxGenericValidator(&bRegenerateMesh));
		XRCCTRL(AbaqusInput, "ElementType", wxRadioBox)->SetValidator(wxGenericValidator(&iElementType));

		if (AbaqusInput.ShowModal() == wxID_OK)
		{
			if (dialog.ShowModal() == wxID_OK)
			{
				Command << "tension = CLinearTransformation()" << endl;
				Command << "tension.AddScale(" << ConvertString(XScale) << "," << ConvertString(YScale) << "," << ConvertString(ZScale) << ")" << endl;

				Command << "deformer = TextileDeformerAbaqus()" << endl;
				Command << "deformer.SetIncludePlates(" << bIncludePlates << ")" << endl;
				Command << "deformer.AddDeformationStep(tension)" << endl;

				//#deformer.SetFortranProgram('LinearElasticUMAT.for')
				//Command << "deformer.SetMaterial('ISO', [" << ConvertString(YoungsModulus) << "," << ConvertString(PoissonsRatio) << "])" << endl;
				//#deformer.SetSimulationFilesPrefix(Abaquspre)

				//name = baseName + '.tg3'
				//ReadFromXML(name)
				Command << "textile = GetTextile('" + TextileName + "')" << endl;

				//#name = deformer.GetSimulationFilesPrefix()
				//Command << "deformer.CreateAbaqusInputFile(textile, 'TestAbaqus' + '.inp')" << endl;
				double Tolerance;
				IntersectionTolerance.ToDouble( &Tolerance );
				Command << "deformer.CreateAbaqusInputFile(textile, r\'" << ConvertString(dialog.GetPath()) << "'," << bRegenerateMesh << "," << iElementType << "," 
															<< bAdjustIntersections << "," << Tolerance <<  ")" << endl;

				SendPythonCode(Command.str());
			}
		}
	}

	

/*if not deformer.CreateAbaqusInputFile(textile, baseName + '.inp'):
	raise RuntimeError('Unable to create ABAQUS input file')
else:
    print 'Abaqus input files are complete now.'*/


}

void CTexGenMainFrame::OnSaveABAQUSVoxels(wxCommandEvent& event)
{

	string TextileName = GetTextileSelection();
	stringstream Command;

	wxString XVoxels = wxT("50");
	wxString YVoxels = wxT("50");
	wxString ZVoxels = wxT("50");
	wxString XOffset = wxT("0.0");
	
	bool bOutputMatrix = true;
	bool bOutputYarns = false;
	int  iBoundaryConditions = 0;
	int iElementType = 0;

	wxFileDialog dialog
	(
		this,
		wxT("Save Abaqus file"),
		wxGetCwd(),
		wxEmptyString,
		wxT("Abaqus input file (*.inp)|*.inp"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
	);
	dialog.CentreOnParent();
	
	CAbaqusVoxelInput AbaqusVoxelInput(this);

	{
		XRCCTRL(AbaqusVoxelInput, "XVoxelCount", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &XVoxels));
		XRCCTRL(AbaqusVoxelInput, "YVoxelCount", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &YVoxels));
		XRCCTRL(AbaqusVoxelInput, "ZVoxelCount", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &ZVoxels));
		XRCCTRL(AbaqusVoxelInput, "OutputMatrix", wxCheckBox)->SetValidator(wxGenericValidator(&bOutputMatrix));
		XRCCTRL(AbaqusVoxelInput, "OutputYarns", wxCheckBox)->SetValidator(wxGenericValidator(&bOutputYarns));
		XRCCTRL(AbaqusVoxelInput, "PeriodicBoundaries", wxRadioBox)->SetValidator(wxGenericValidator(&iBoundaryConditions));
		XRCCTRL(AbaqusVoxelInput, "ElementType", wxRadioBox)->SetValidator(wxGenericValidator(&iElementType));
		XRCCTRL(AbaqusVoxelInput, "Offset", wxTextCtrl)->SetValidator(RangeValidator(&XOffset, 0.0, 1.0));

		if (AbaqusVoxelInput.ShowModal() == wxID_OK)
		{
			if (dialog.ShowModal() == wxID_OK)
			{
				if ( !bOutputMatrix && !bOutputYarns )
				{
					TGERROR("No output selected");
					return;
				}

				if ( iBoundaryConditions == SHEARED_BC )
				{
					Command << "Vox = CShearedVoxelMesh('CShearedPeriodicBoundaries')" << endl;
				}
				else if ( iBoundaryConditions == STAGGERED_BC )
				{
					Command << "Vox = CStaggeredVoxelMesh('CStaggeredPeriodicBoundaries')" << endl;
					Command << "Vox.SetOffset(" << ConvertString(XOffset) << ")" << endl;
				}
				else
					Command << "Vox = CRectangularVoxelMesh('CPeriodicBoundaries')" << endl;
				Command << "Vox.SaveVoxelMesh(GetTextile('" + TextileName + "'), r\'" << ConvertString(dialog.GetPath()) << "', " << ConvertString(XVoxels) << "," << ConvertString(YVoxels) << "," << ConvertString(ZVoxels) 
					<< "," << bOutputMatrix << "," << bOutputYarns << ","<< iBoundaryConditions << "," << iElementType << ")" << endl;

				SendPythonCode(Command.str());
			}
			
		}
	}
	

/*if not deformer.CreateAbaqusInputFile(textile, baseName + '.inp'):
	raise RuntimeError('Unable to create ABAQUS input file')
else:
    print 'Abaqus input files are complete now.'*/


}

void CTexGenMainFrame::OnPeriodicBoundaries(wxCommandEvent& event)
{
	int selection = event.GetSelection();
}

void CTexGenMainFrame::OnSaveTetgenMesh( wxCommandEvent& event )
{
	TGLOG("CTexGenMainFrame::OnSaveTetgenMesh");
	wxFileDialog dialog
	(
		this,
		wxT("Save Tetgen Mesh file"),
		wxGetCwd(),
		wxEmptyString,
		wxT("Abaqus input file (*.inp)|*.inp"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
	);
	dialog.CentreOnParent();

	wxString params = wxT("pqAY");
	wxString seed = wxT("0.1");
	bool bPeriodic = true;
	wxDialog TetgenInput;
	if (wxXmlResource::Get()->LoadDialog(&TetgenInput, this, wxT("TetgenOptions")))
	{
		XRCCTRL(TetgenInput, "Param", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NONE, &params));
		XRCCTRL(TetgenInput, "SeedSize", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &seed));
		XRCCTRL(TetgenInput, "Periodic", wxCheckBox)->SetValidator(wxGenericValidator(&bPeriodic));
		if (TetgenInput.ShowModal() == wxID_OK && dialog.ShowModal() == wxID_OK)
		{
			string textileName = GetTextileSelection();
			auto tetMesh = new CTetgenMesh(0.1);
			auto textile = CTexGen::Instance().GetTextile(textileName);
			auto path = ConvertString(dialog.GetPath());
			auto prms = ConvertString(params);
			tetMesh->SaveTetgenMesh(*textile, path, prms, bPeriodic);
		}
	}
}

void CTexGenMainFrame::UpdateWindowChecks()
{
	// Find out if the widget should be checked or not
	bool bControls = m_Manager.GetPane(m_pControls).IsShown();
	bool bLogs = m_Manager.GetPane(m_pLogNotebook).IsShown();
	bool bOutliner = m_Manager.GetPane(m_pOutliner).IsShown();
	GetMenuBar()->Check(ID_ToggleControls, bControls);
	GetMenuBar()->Check(ID_ToggleLogWindow, bLogs);
	GetMenuBar()->Check(ID_ToggleOutliner, bOutliner);

}

void CTexGenMainFrame::OnWindow(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	case ID_ToggleControls:
		{
			wxAuiPaneInfo& Pane = m_Manager.GetPane(m_pControls);
			Pane.Show(!Pane.IsShown());
		}
		break;
	case ID_ToggleLogWindow:
		{
			wxAuiPaneInfo& Pane = m_Manager.GetPane(m_pLogNotebook);
			Pane.Show(!Pane.IsShown());
		}
		break;
	case ID_ToggleOutliner:
		{
			wxAuiPaneInfo& Pane = m_Manager.GetPane(m_pOutliner);
			Pane.Show(!Pane.IsShown());
		}
		break;
	}
    m_Manager.Update();
	UpdateWindowChecks();
}

CTexGenRenderWindow *CTexGenMainFrame::GetRenderWindow(string WindowName)
{
	if (WindowName.empty())
	{
		int iPage = m_pViewerNotebook->GetSelection();
		if (iPage != -1)
			WindowName = ConvertString(m_pViewerNotebook->GetPageText(iPage));
	}
	// Find the window with given name
	map<string, CTexGenRenderWindow*>::iterator itRenderWindow;
	itRenderWindow = m_RenderWindows.find(WindowName);
	if (itRenderWindow != m_RenderWindows.end())
	{
		return itRenderWindow->second;
	}
	return NULL;
}

CTexGenRenderWindow *CTexGenMainFrame::CreateRenderWindow(string WindowName)
{
	if (WindowName.empty())
	{
		string DefaultName = "RenderWindow";
		WindowName = DefaultName;
		int iNumber = 1;
		// Check that the entry does not exist, and the name given is not blank
		while (m_RenderWindows.find(WindowName) != m_RenderWindows.end())
		{
			WindowName = DefaultName + "-" + stringify(++iNumber);
		}
	}
	// Check that the entry does not exist, and the name given is not blank
	if (m_RenderWindows.find(WindowName) != m_RenderWindows.end())
	{
		TGERROR("Unable to create render window, window with that name already exists: " + WindowName);
		return NULL;
	}

	CTexGenRenderWindow *pRenderWindow = new CTexGenRenderWindow(m_pViewerNotebook, wxID_ANY);
	// Attach the modeller to it
	pRenderWindow->AttachModeller(WindowName);
	// Add the section to the list using the name as the key
	m_RenderWindows[WindowName] = pRenderWindow;
	// This line must be after the render window has been added to m_RenderWindows
	// otherwise the function to update the rendering page will not work correctly
	m_pViewerNotebook->AddPage(pRenderWindow, ConvertString(WindowName), true);
	TGLOG("Created render window \"" << WindowName << "\"");
	return pRenderWindow;
}

bool CTexGenMainFrame::DeleteRenderWindow(string WindowName)
{
	if (WindowName.empty())
	{
		int iSelected = m_pViewerNotebook->GetSelection();
		if (iSelected == -1)
			return false;
		WindowName = ConvertString(m_pViewerNotebook->GetPageText(iSelected));
	}
	map<string, CTexGenRenderWindow*>::iterator itRenderWindow;
	itRenderWindow = m_RenderWindows.find(WindowName);
	if (itRenderWindow == m_RenderWindows.end())
		return false;
	size_t i;
	for (i=0; i<m_pViewerNotebook->GetPageCount(); ++i)
	{
		if (m_pViewerNotebook->GetPage(i) == itRenderWindow->second)
		{
			m_pViewerNotebook->RemovePage(i);
			break;
		}
	}
	itRenderWindow->second->CleanUp();
	itRenderWindow->second->Delete();
	m_RenderWindows.erase(itRenderWindow);
	return true;
}

void CTexGenMainFrame::SendPythonCode(string Code)
{
	if (m_ScriptRecordFile.is_open())
	{
		m_ScriptRecordFile << Code << endl;
	}
	stringstream StringStream;
	StringStream << Code;
	string Line;
	string Prefix;
	int iLine = 0;
	while (StringStream.good())
	{
		Prefix = "<" + stringify(++iLine) + "> ";
		getline(StringStream, Line);
		if (!Line.empty())
		{
			ReceiveOutput(Prefix + Line + "\n", OUTPUT_PYTHON, false, false);
		}
	}

	/*if (!m_PythonWrapper.SendCodeBlock(Code))
	{
		ReceiveOutput("<!> Error compiling code!", OUTPUT_PYTHON, true, false);
	}//*/
}

void CTexGenMainFrame::OnCreateEmptyTextile()
{
	wxTextEntryDialog dlg(this, wxT("Please enter the name of the textile to create (or leave blank for default):"), wxT("Textile name"));
	if (dlg.ShowModal() == wxID_OK)
	{
		auto textileName = ConvertString(dlg.GetValue());
		if (textileName.empty())
		{
			TGLOG("AddTextile(CTextile())");
			CTexGen::Instance().AddTextile(CTextile());
		}
		else
		{
			TGLOG("AddTextile('" << textileName << "', CTextile())");
			CTexGen::Instance().AddTextile(textileName, CTextile());
		}
	}
}

void CTexGenMainFrame::OnDeleteTextile()
{
	auto textileName = GetTextileSelection();
	if (!textileName.empty())
	{
		if (wxMessageBox(wxT("Do you wish to delete the textile?"), wxT("Delete textile"), wxNO_DEFAULT | wxYES_NO | wxICON_INFORMATION, this) == wxYES)
		{
			TGLOG("DeleteTextile('" + textileName + "')");
			CTexGen::Instance().DeleteTextile(textileName);
		}
	}
}

void CTexGenMainFrame::OnTextiles(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	case ID_DeleteTextile:
		OnDeleteTextile(); break;
	case ID_EditTextile:
		{
			string TextileName = GetTextileSelection();
			if (!TextileName.empty())
			{
				CTextile* pTextile = CTexGen::Instance().GetTextile(TextileName);
				string Type = pTextile->GetType();
				if (pTextile && (Type == "CTextileWeave2D" || Type == "CShearedTextileWeave2D" ||
					Type == "CTextileOrthogonal" || Type == "CTextileAngleInterlock" || Type == "CTextileOffsetAngleInterlock" || Type == "CTextileLayerToLayer"))
				{
					string Command;
					if (Type == "CTextileWeave2D" || Type == "CShearedTextileWeave2D")
					{
						CWeaveWizard Wizard(this, wxID_ANY);
						if (Type == "CTextileWeave2D")
							Wizard.LoadSettings(*((CTextileWeave2D*)pTextile));
						else
							Wizard.LoadSettings(*((CShearedTextileWeave2D*)pTextile));

						if ( Wizard.RunIt() )
						{
							Command = Wizard.GetCreateTextileCommand(TextileName);
						}
					}
					else // if (pTextile->GetType() == "CTextileWeave3D")
					{
						CWeaveWizard3D Wizard(this, wxID_ANY);
						Wizard.LoadSettings(*((CTextile3DWeave*)pTextile));
						if ( Wizard.RunIt() )
						{
							Command = Wizard.GetCreateTextileCommand(TextileName);
						}
					}
					
					if (!Command.empty())
					{
						SendPythonCode(Command);
						RefreshTextile(TextileName);
					}
				}
			}
		}
		break;
	case ID_CreateEmptyTextile:
		OnCreateEmptyTextile(); break;
	case ID_Create2DWeave:
		{
			CWeaveWizard Wizard(this, wxID_ANY); 
			if (Wizard.RunIt())
			{
				string Command = Wizard.GetCreateTextileCommand();
				if (!Command.empty())
				{
					SendPythonCode(Command);
				}
			}
		}
		break;
	case ID_GeometrySolve:
		{
			OnGeometrySolve(event);
		}
		break;

	case ID_Create3DTextile:
		{
			CWeaveWizard3D Wizard(this, wxID_ANY);
			if ( Wizard.RunIt() )
			{
				string Command = Wizard.GetCreateTextileCommand();
				if ( !Command.empty() )
				{
					SendPythonCode(Command);
				}
			}
		}
		break;
	case ID_CreateLayeredTextile:
		{
			CTextileLayersDialog Dialog;
			if ( Dialog.ShowModal() == wxID_OK )
			{
				vector<string> LayerNames;
				Dialog.GetLayerNames( LayerNames );
				if ( !LayerNames.empty() ) 
				{
					// Create python code with list of names
					string Command = ConvertMultiWeaveLayered( LayerNames );
					SendPythonCode( Command );
				}
			}
		}
		break;
	case ID_SetLayerOffsets:
		{
			string TextileName = GetTextileSelection();
			if (!TextileName.empty())
			{
				CTextile* pTextile = CTexGen::Instance().GetTextile(TextileName);
				string Type = pTextile->GetType();
				if ( Type != "CTextileLayered" )
				{
					TGERROR("Cannot set layers: not a layered textile");
					break;
				}
				
				CLayersOffsetDialog Dialog;
				XYZ Min,Max;
				CDomain* Domain = pTextile->GetDomain();
				((CDomainPlanes*)Domain)->GetBoxLimits(Min, Max);
				XY DomainSize( Max.x-Min.x, Max.y-Min.y );
				Dialog.LoadSettings( dynamic_cast<CTextileLayered*>(pTextile)->GetOffsets(), DomainSize );
				
				if (Dialog.ShowModal() == wxID_OK)
				{
					vector<XY> LayerOffsets;
					int iOption = Dialog.GetOption();
					stringstream StringStream;

					switch ( iOption )
					{
						case CONSTANT:
							{
							XY Offset;
							Dialog.GetConstantOffset( Offset );
							StringStream << "textile = GetTextile('" << TextileName << "')" << endl;
							StringStream << "LayeredTextile = textile.GetLayeredTextile()" << endl;
							StringStream << "Offset = XY(" << Offset << ")" << endl;
							StringStream << "LayeredTextile.SetOffsets( Offset )" << endl;
							break;
							}
						case RANDOM:
						case EDIT:
							Dialog.GetEditOffsets( LayerOffsets );
							StringStream << "textile = GetTextile('" << TextileName << "')" << endl;
							StringStream << "LayeredTextile = textile.GetLayeredTextile()" << endl;
							StringStream << "Offsets = XYVector()" << endl;
							for (int i=0; i<(int)LayerOffsets.size(); ++i)
							{
								StringStream << "Offsets.append(XY(" << LayerOffsets[i] << "))" << endl;
							}
							StringStream << "LayeredTextile.SetOffsets( Offsets )" << endl;
							break;
					}
					SendPythonCode( StringStream.str() );
					RefreshTextile( TextileName );
				}
			}
		}
		break;
	case ID_NestLayers:
	case ID_MaxNestLayers:
		{
			string TextileName = GetTextileSelection();
			if (!TextileName.empty())
			{
				CTextile* pTextile = CTexGen::Instance().GetTextile(TextileName);
				string Type = pTextile->GetType();
				if ( Type != "CTextileLayered" )
				{
					TGERROR("Cannot nest layers: not a layered textile");
					break;
				}

				stringstream StringStream;
				StringStream << "textile = GetTextile('" << TextileName << "')" << endl;
				StringStream << "LayeredTextile = textile.GetLayeredTextile()" << endl;
				if ( event.GetId() == ID_NestLayers )
					StringStream << "LayeredTextile.NestLayers()" << endl;
				else
					StringStream << "LayeredTextile.MaxNestLayers()" << endl;
				SendPythonCode( StringStream.str() );
				RefreshTextile( TextileName );
			}
		}
		break;
	}
}

void CTexGenMainFrame::OnGeometrySolve(wxCommandEvent& event)
{
	string TextileName = GetTextileSelection();

//	int iMeshType = 0;
	CGeometrySolver Solver;	// Create a geometry solver object just to get the default values

	wxString SeedSize; SeedSize << Solver.GetSeed();
	wxString TensileStress; TensileStress << Solver.GetTensileStress();
	wxString LongitudinalBendingModulus; LongitudinalBendingModulus << Solver.GetLongitudinalBendingModulus();
	wxString TransverseBendingModulus; TransverseBendingModulus << Solver.GetTransverseBendingModulus();
	wxString ContactStiffness; ContactStiffness << Solver.GetContactStiffness();
	wxString DisabledStiffness; DisabledStiffness << Solver.GetDisabledStiffness();
	bool bCreateCopy = false;
	wxDialog GeometrySolveOptions;
	if (wxXmlResource::Get()->LoadDialog(&GeometrySolveOptions, this, wxT("GeometrySolveOptions")))
	{
		XRCCTRL(GeometrySolveOptions, "SeedSize", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &SeedSize));
		XRCCTRL(GeometrySolveOptions, "TensileStress", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &TensileStress));
		XRCCTRL(GeometrySolveOptions, "LongitudinalBendingModulus", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &LongitudinalBendingModulus));
		XRCCTRL(GeometrySolveOptions, "TransverseBendingModulus", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &TransverseBendingModulus));
		XRCCTRL(GeometrySolveOptions, "ContactStiffness", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &ContactStiffness));
		XRCCTRL(GeometrySolveOptions, "DisabledStiffness", wxTextCtrl)->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &DisabledStiffness));
		XRCCTRL(GeometrySolveOptions, "CreateCopyCheckBox", wxCheckBox)->SetValidator(wxGenericValidator(&bCreateCopy));
		if (GeometrySolveOptions.ShowModal() == wxID_OK)
		{
			stringstream Command;
			Command << "solver = CGeometrySolver()" << endl;
			if (!SeedSize.IsEmpty())
				Command << "solver.SetSeed(" << ConvertString(SeedSize) << ")" << endl;
			if (!TensileStress.IsEmpty())
				Command << "solver.SetTensileStress(" << ConvertString(TensileStress) << ")" << endl;
			if (!LongitudinalBendingModulus.IsEmpty())
				Command << "solver.SetLongitudinalBendingModulus(" << ConvertString(LongitudinalBendingModulus) << ")" << endl;
			if (!TransverseBendingModulus.IsEmpty())
				Command << "solver.SetTransverseBendingModulus(" << ConvertString(TransverseBendingModulus) << ")" << endl;
			if (!ContactStiffness.IsEmpty())
				Command << "solver.SetContactStiffness(" << ConvertString(ContactStiffness) << ")" << endl;
			if (!DisabledStiffness.IsEmpty())
				Command << "solver.SetDisabledStiffness(" << ConvertString(DisabledStiffness) << ")" << endl;
			Command << "solver.CreateSystem('" << TextileName << "')" << endl;
			Command << "solver.SolveSystem()" << endl;
			if (bCreateCopy)
				Command << "solver.GetDeformedCopyOfTextile()" << endl;
			else
				Command << "solver.DeformTextile()" << endl;
			SendPythonCode(Command.str());
			RefreshTextile(TextileName);
		}
/*
		if (MeshOptions.ShowModal() == wxID_OK)
		{
			wxFileDialog dialog
			(
				this,
				wxT("Save Mesh file"),
				wxGetCwd(),
				wxEmptyString,
				wxT("VTK unstructured grid file (*.vtu)|*.vtu|"
					"ABAQUS input file (*.inp)|*.inp"),
				wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
			);
			dialog.CentreOnParent();
			if (dialog.ShowModal() == wxID_OK)
			{
				stringstream Command;
				Command << "mesher = CMesher()" << endl;
				if (iMeshType == 1)
					Command << "mesher.SetHybrid(True)" << endl;
				if (iElementOrder == 1)
					Command << "mesher.SetQuadratic(True)" << endl;
				if (bPeriodic)
					Command << "mesher.SetPeriodic(True)" << endl;
				if (!SeedSize.IsEmpty())
					Command << "mesher.SetSeed(" << ConvertString(SeedSize) << ")" << endl;
				if (!MergeTolerance.IsEmpty())
					Command << "mesher.SetMergeTolerance(" << ConvertString(MergeTolerance) << ")" << endl;
				Command << "mesher.CreateMesh(r'" << TextileName << "')" << endl;
				switch (dialog.GetFilterIndex())
				{
				case 0:
					Command << "mesher.SaveVolumeMeshToVTK(r'" << ConvertString(dialog.GetPath()) << "')" << endl;
					break;
				case 1:
					Command << "mesher.SaveVolumeMeshToABAQUS(r'" << ConvertString(dialog.GetPath()) << "')" << endl;
					break;
				}
				SendPythonCode(Command.str());
			}
		}*/
	}
}

void CTexGenMainFrame::OnModeller(wxCommandEvent& event)
{
	CTexGenRenderWindow* pRenderWindow = GetRenderWindow();
	CModeller* pModeller = NULL;
	if (pRenderWindow)
	{
		pModeller = pRenderWindow->GetModeller();
	}
	if ( !pModeller )
	{
		wxMessageBox(wxT("Please create textile before using Modeller options"), wxT("Modeller Error"), wxOK | wxICON_ERROR, this);
		return;
	}
	switch (event.GetId())
	{
	case ID_CreateYarn:
		pModeller->CreateYarn();
		break;
	case ID_AssignSection:
		pModeller->AssignSectionToSelectedObjects();
		break;
	case ID_AssignInterpolation:
		pModeller->AssignInterpolationToSelectedObjects();
		break;
	case ID_AssignRepeats:
		pModeller->AssignRepeatsToSelectedObjects();
		break;
	case ID_AssignProperties:
		pModeller->AssignPropertiesToSelectedObjects();
		break;
	case ID_AssignMatrixProperties:
		pModeller->AssignMatrixProperties();
		break;
	case ID_YarnFibreVolumeFraction:
		pModeller->DisplayVolumeFractionForSelectedObjects();
		break;
/*	case ID_ShowOutliner:
		pModeller->ShowOutliner(true);
		break;*/
	case ID_SelectTool:
		{
			pModeller->SetActiveTool(TOOL_SELECT);
			m_pControls->UpdateModellingPage(pModeller);
		}
		break;
	case ID_MoveTool:
		{
			pModeller->SetActiveTool(TOOL_MOVE);
			m_pControls->UpdateModellingPage(pModeller);
		}
		break;
	case ID_RotateTool:
		{
			pModeller->SetActiveTool(TOOL_ROTATE);
			m_pControls->UpdateModellingPage(pModeller);
		}
		break;
	case ID_ScaleTool:
		{
			pModeller->SetActiveTool(TOOL_SCALE);
			m_pControls->UpdateModellingPage(pModeller);
		}
		break;
	case ID_FilterNodes:
		{
			pModeller->SetSelectNodes(event.IsChecked());
			m_pControls->UpdateModellingPage(pModeller);
		}
		break;
	case ID_FilterPath:
		{
			pModeller->SetSelectPaths(event.IsChecked());
			m_pControls->UpdateModellingPage(pModeller);
		}
		break;
	case ID_FilterSurface:
		{
			pModeller->SetSelectSurfaces(event.IsChecked());
			m_pControls->UpdateModellingPage(pModeller);
		}
		break;
	case ID_Snap:
		{
			pModeller->GetWidget()->SetSnap(event.IsChecked());
			m_pControls->UpdateModellingPage(pModeller);
		}
		break;
	case ID_Relative:
		{
			pModeller->GetWidget()->SetRelativeSnap(event.IsChecked());
			m_pControls->UpdateModellingPage(pModeller);
		}
		break;
/*	case ID_SnapSize:
		{
			pModeller->GetWidget()->SetSnapSize();
			m_pControls->UpdateModellingPage(pModeller);
		}
		break;*/
	}
}

void CTexGenMainFrame::OnPosition(wxCommandEvent &event)
{
	if (m_pControls && m_pControls->m_bUpdatingPositionText)
		return;
	int iCoord;
	switch (event.GetId())
	{
	case ID_PositionX:
		iCoord = 0;
		break;
	case ID_PositionY:
		iCoord = 1;
		break;
	case ID_PositionZ:
		iCoord = 2;
		break;
	default:
		return;
	}
	wxTextCtrl* pTextCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
	if (pTextCtrl)
	{
		wxValidator* pValidator = pTextCtrl->GetValidator();
		if (pValidator)
		{
			pValidator->TransferFromWindow();
			wxString Text = pTextCtrl->GetValue();
			double dValue;
			if (Text.ToDouble(&dValue))
			{
				CTexGenRenderWindow* pRenderWindow = GetRenderWindow();
				if (pRenderWindow)
				{
					CModeller* pModeller = pRenderWindow->GetModeller();
					if (pModeller)
						pModeller->SetPosition(iCoord, dValue);
				}
			}
		}
	}
}

void CTexGenMainFrame::OnSnapSize(wxCommandEvent &event)
{
	wxTextCtrl* pTextCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
	if (pTextCtrl)
	{
		wxValidator* pValidator = pTextCtrl->GetValidator();
		if (pValidator)
		{
			pValidator->TransferFromWindow();
			wxString Text = pTextCtrl->GetValue();
			double dValue;
			if (Text.ToDouble(&dValue))
			{
				CTexGenRenderWindow* pRenderWindow = GetRenderWindow();
				if (pRenderWindow)
				{
					CModeller* pModeller = pRenderWindow->GetModeller();
					if (pModeller)
						pModeller->GetWidget()->SetSnapSize(dValue);
				}
			}
		}
	}
}

void CTexGenMainFrame::EditDomain()
{
	string TextileName = GetTextileSelection();
	if (!TextileName.empty())
	{
		CTextile* pTextile = TEXGEN.GetTextile(TextileName);
		if (pTextile)
		{
			const CDomain* pDomain = pTextile->GetDomain();
			if (pDomain && pDomain->GetType() == "CDomainPlanes")
			{
				CDomainPlanesDialog Dialog(this, wxID_ANY);
				Dialog.LoadSettings(*((CDomainPlanes*)pDomain));
				if (Dialog.ShowModal() == wxID_OK)
				{
					string Command = Dialog.GetCreateDomainCommand();
					if (!Command.empty())
					{
						Command += "GetTextile('" + TextileName + "').AssignDomain(domain)";
						SendPythonCode(Command);
						RefreshTextile(TextileName);
					}
				}
			}
		}
	}
}

void CTexGenMainFrame::OnDomains(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	case ID_DeleteDomain:
		{
			string TextileName = GetTextileSelection();
			if (!TextileName.empty())
			{
				string Command = "textile = GetTextile('" + TextileName + "')\n";
				Command += "textile.RemoveDomain()";
				SendPythonCode(Command);
				RefreshTextile(TextileName);
			}
		}
		break;
	case ID_EditDomain:
		EditDomain();
		break;
	case ID_CreateDomainPlanes:
		{
			CDomainPlanesDialog Dialog(this, wxID_ANY);
			if (Dialog.ShowModal() == wxID_OK)
			{
				string TextileName = GetTextileSelection();
				if (!TextileName.empty())
				{
					string Command = Dialog.GetCreateDomainCommand();
					if (!Command.empty())
					{
						Command += "GetTextile('" + TextileName + "').AssignDomain(domain)";
						SendPythonCode(Command);
						RefreshTextile(TextileName);
					}
				}
			}
		}
		break;
	case ID_CreateDomainBox:
		{
			CDomainPlanesDialog Dialog(this, wxID_ANY);
			if (Dialog.CreateBox())
			{
				string TextileName = GetTextileSelection();
				if (!TextileName.empty())
				{
					string Command = Dialog.GetCreateDomainCommand();
					if (!Command.empty())
					{
						Command += "GetTextile('" + TextileName + "').AssignDomain(domain)";
						SendPythonCode(Command);
						RefreshTextile(TextileName);
					}
				}
			}
		}
		break;
	}
}

void CTexGenMainFrame::OnRendering(wxCommandEvent& event)
{
	string TextileName = GetTextileSelection();
	if (TextileName.empty())
		return;
	CTextile* pTextile = CTexGen::Instance().GetTextile(TextileName);
	if (!pTextile)
		return;
	string Command;
	switch (event.GetId())
	{
	case ID_RenderNodes:
		if (event.IsChecked())
			Command = "GetRenderWindow('" + TextileName + "').RenderNodes('" + TextileName + "')";
		else
		{
			Command = "GetRenderWindow('" + TextileName + "').RemoveNodes()";
		}
		break;
	case ID_RenderPaths:
		if (event.IsChecked())
			Command = "GetRenderWindow('" + TextileName + "').RenderPaths('" + TextileName + "')";
		else
			Command = "GetRenderWindow('" + TextileName + "').RemovePaths()";
		break;
	case ID_RenderSurface:
		if (event.IsChecked())
			Command = "GetRenderWindow('" + TextileName + "').RenderTextile('" + TextileName + "')";
		else
			Command = "GetRenderWindow('" + TextileName + "').RemoveTextiles()";
		break;
	case ID_RenderVolume:
		if (event.IsChecked())
			Command = "GetRenderWindow('" + TextileName + "').RenderMesh('" + TextileName + "')";
		else
			Command = "GetRenderWindow('" + TextileName + "').RemoveMeshes()";
		break;
	case ID_RenderInterference:
		if (event.IsChecked())
			Command = "GetRenderWindow('" + TextileName + "').RenderInterference('" + TextileName + "')";
		else
			Command = "GetRenderWindow('" + TextileName + "').RemoveInterference()";
		break;
	case ID_RenderInterferenceDepth:
		if (event.IsChecked())
			Command = "GetRenderWindow('" + TextileName + "').RenderInterference('" + TextileName + "', True)";
		else
			Command = "GetRenderWindow('" + TextileName + "').RemoveInterferenceDepth()";
		break;
	case ID_RenderOrientation:
		if (event.IsChecked())
			Command = "GetRenderWindow('" + TextileName + "').RenderGrid('" + TextileName + "')";
		else
			Command = "GetRenderWindow('" + TextileName + "').RemoveOrientations()";
		break;
	case ID_RenderDomain:
		if (event.IsChecked())
			Command = "GetRenderWindow('" + TextileName + "').RenderDomain('" + TextileName + "')";
		else
			Command = "GetRenderWindow('" + TextileName + "').RemoveDomains()";
		break;
	case ID_RenderDomainAxes:
		if (event.IsChecked())
			Command = "GetRenderWindow('" + TextileName + "').RenderDomainAxes('" + TextileName + "')";
		else
			Command = "GetRenderWindow('" + TextileName + "').RemoveDomainAxes()";
		break;
	case ID_RenderAxes:
		if (event.IsChecked())
			Command = "GetRenderWindow('" + TextileName + "').RenderAxes()";
		else
			Command = "GetRenderWindow('" + TextileName + "').RemoveAxes()";
		break;
	case ID_XRay:
		if (event.IsChecked())
			Command = "GetRenderWindow('" + TextileName + "').SetXRay(True)\n";
		else
			Command = "GetRenderWindow('" + TextileName + "').SetXRay(False)\n";
		{
			CTexGenRenderWindow* pRenderWindow = GetRenderWindow(TextileName);
			if (pRenderWindow)
			{
				CTexGenRenderer* pRenderer = pRenderWindow->GetRenderer();
				if (pRenderer)
				{
					if (pRenderer->GetNumProps(CTexGenRenderer::PROP_SURFACE))
					{
						Command += "GetRenderWindow('" + TextileName + "').RemoveTextiles()\n";
						Command += "GetRenderWindow('" + TextileName + "').RenderTextile('" + TextileName + "')";
					}
				}
			}
		}
		break;
	case ID_ParallelProjection:
		if (event.IsChecked())
		{
			Command = "GetRenderWindow('" + TextileName + "').SetParallelProjection(True)\n";
		}
		else
		{
			Command = "GetRenderWindow('" + TextileName + "').SetParallelProjection(False)\n";
		}
/*		{
			CTexGenRenderWindow* pRenderWindow = GetRenderWindow(TextileName);
			if (pRenderWindow)
			{
				CTexGenRenderer* pRenderer = pRenderWindow->GetRenderer();
				if (pRenderer)
				{
					if (pRenderer->GetNumProps(CTexGenRenderer::PROP_SURFACE))
					{
						Command += "GetRenderWindow('" + TextileName + "').RemoveTextiles()\n";
						Command += "GetRenderWindow('" + TextileName + "').RenderTextile('" + TextileName + "')";
					}
				}
			}
		}*/
		break;
	case ID_TrimtoDomain:
		if (event.IsChecked())
			Command = "GetRenderWindow('" + TextileName + "').SetTrimToDomain(True)\n";
		else
			Command = "GetRenderWindow('" + TextileName + "').SetTrimToDomain(False)\n";
		{
			CTexGenRenderWindow* pRenderWindow = GetRenderWindow(TextileName);
			if (pRenderWindow)
			{
				CTexGenRenderer* pRenderer = pRenderWindow->GetRenderer();
				if (pRenderer)
				{
					if (pRenderer->GetNumProps(CTexGenRenderer::PROP_SURFACE))
					{
						Command += "GetRenderWindow('" + TextileName + "').RemoveTextiles()\n";
						Command += "GetRenderWindow('" + TextileName + "').RenderTextile('" + TextileName + "')\n";
					}
					if ( pRenderer->GetNumProps(CTexGenRenderer::PROP_INTERFERENCE) )
					{
						Command += "GetRenderWindow('" + TextileName + "').RemoveInterference()\n";
						Command += "GetRenderWindow('" + TextileName + "').RenderInterference('" + TextileName + "')\n";
					}
					if ( pRenderer->GetNumProps(CTexGenRenderer::PROP_INTERFERENCE_DEPTH) )
					{
						Command += "GetRenderWindow('" + TextileName + "').RemoveInterferenceDepth()\n";
						Command += "GetRenderWindow('" + TextileName + "').RenderInterference('" + TextileName + "', True)\n";
					}
					if ( pRenderer->GetNumProps(CTexGenRenderer::PROP_ORIENTATION) )
					{
						Command += "GetRenderWindow('" + TextileName + "').RemoveOrientations()\n";
						Command += "GetRenderWindow('" + TextileName + "').RenderGrid('" + TextileName + "')\n";
					}
				}
			}
		}
		break;
	case ID_ChangeBackgroundColor:
		{
			string Color = GetUserSelectedColor();
			if (!Color.empty())
				Command = "GetRenderWindow('" + TextileName + "').SetBackgroundColor(" + Color + ")";
		}
		break;
	case ID_ChangeSurfaceColor:
		{
			string Color = GetUserSelectedColor();
			if (!Color.empty())
				Command = "GetRenderWindow('" + TextileName + "').SetPropsColor(CTexGenRenderer.PROP_SURFACE, " + Color + ")";
		}
		break;
	}
	if (!Command.empty())
	{
		SendPythonCode(Command);
	}
	UpdateRenderingPage();
}

void CTexGenMainFrame::OnPython(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	case ID_RunScript:
		{
			wxFileDialog dialog
			(
				this,
				wxT("Run python script"),
				wxGetCwd(),
				wxEmptyString,
				wxT("Python script (*.py)|*.py"),
				wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR
			);

			dialog.CentreOnParent();

			if (dialog.ShowModal() == wxID_OK)
			{
				string Command;
				Command = "execfile(r\"";
				Command += ConvertString(dialog.GetPath());
				Command += "\")";
				SendPythonCode(Command);
			}
		}
		break;
	case ID_RecordMacro:
		{
			if (m_ScriptRecordFile.is_open())
			{
				m_ScriptRecordFile.close();
			}
			else
			{
				wxFileDialog dialog
				(
					this,
					wxT("Record python script"),
					wxGetCwd(),
					wxEmptyString,
					wxT("Python script (*.py)|*.py"),
					wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
				);

				dialog.CentreOnParent();

				if (dialog.ShowModal() == wxID_OK)
				{
					m_ScriptRecordFile.open(dialog.GetPath().mb_str());
					if (!m_ScriptRecordFile.is_open())
					{
						wxMessageBox(wxT("Unable to open file for recording:\n") + dialog.GetPath(), wxT("Error saving file"), wxOK | wxICON_ERROR, this);
					}
					else
					{
						m_ScriptRecordFile << "# Script recorded by TexGen v" << TEXGEN.GetVersion() << endl;
					}
				}
			}
			if (m_pControls)
				m_pControls->UpdatePythonPage(m_ScriptRecordFile.is_open());
		}
		break;
	}
}

void CTexGenMainFrame::OnLogNotebook(wxAuiNotebookEvent& event)
{
	if (event.GetSelection() != -1)
		m_pLogNotebook->SetPageBitmap(event.GetSelection(), wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_OTHER, wxSize(16, 16)));
}

void CTexGenMainFrame::UpdateRenderingPage()
{
	CTexGenRenderWindow* pRenderWindow = GetRenderWindow();
	if (pRenderWindow)
	{
		CTexGenRenderer* pRenderer = pRenderWindow->GetRenderer();
		if (pRenderer && m_pControls)
		{
			m_pControls->UpdateRenderingPage(pRenderer);
		}
	}
}

void CTexGenMainFrame::UpdateModellingPage()
{
	CTexGenRenderWindow* pRenderWindow = GetRenderWindow();
	if (pRenderWindow)
	{
		CModeller* pModeller = pRenderWindow->GetModeller();
		if (pModeller && m_pControls)
		{
			m_pControls->UpdateModellingPage(pModeller);
		}
	}
}

void CTexGenMainFrame::UpdateOutliner()
{
	CTexGenRenderWindow* pRenderWindow = GetRenderWindow();
	if (pRenderWindow)
	{
		CModeller* pModeller = pRenderWindow->GetModeller();
		if (pModeller && m_pControls)
		{
			pModeller->UpdateOutlinerItems();
			return;
		}
	}

	if (!m_pOutliner)
		return;

	wxTreeCtrl* pTreeCtrl = XRCCTRL(*m_pOutliner, "TreeCtrl", wxTreeCtrl);
	if (pTreeCtrl)
	{
		pTreeCtrl->DeleteAllItems();
	}
}

void CTexGenMainFrame::UpdatePositionText()
{
	CTexGenRenderWindow* pRenderWindow = GetRenderWindow();
	if (pRenderWindow)
	{
		CModeller* pRenderer = pRenderWindow->GetModeller();
		if (pRenderer && m_pControls)
		{
			m_pControls->UpdatePositionText(pRenderer);
		}
	}
}

void CTexGenMainFrame::OnViewerNotebookPageChanged(wxAuiNotebookEvent& event)
{
	if (event.GetSelection() != -1)
	{
		UpdateRenderingPage();
		UpdateModellingPage();
		UpdateOutliner();
	}
}

void CTexGenMainFrame::OnViewerNotebookClose(wxAuiNotebookEvent& event)
{
	// Prevent the event from actually deleting the window, this will be done automagically when the textile is deleted
	event.Veto();
	// Delete the textile...
	auto textileName = ConvertString(m_pViewerNotebook->GetPageText(event.GetSelection()));
	if (!textileName.empty())
	{
		TGLOG("DeleteTextile('" + textileName + "')");
		CTexGen::Instance().DeleteTextile(textileName);
	}
}

/*void CTexGenMainFrame::RefreshDomain(string DomainName)
{
//	map<string, CTexGenRenderWindow*>::iterator itRenderWindow;
//	CTextile* pTextile;
//	for (itRenderWindow = m_RenderWindows.begin(); itRenderWindow != m_RenderWindows.end(); ++itRenderWindow)
//	{
//		pTextile = CTexGen::GetInstance().GetTextile(itRenderWindow->first);
//		if (pTextile && pTextile->GetAssignedDomain() == DomainName)
//		{
//			RefreshTextile(itRenderWindow->first);
//		}
//	}
}*/

bool CTexGenMainFrame::RefreshTextile(string TextileName)
{
	if (TextileName.empty())
	{
		TextileName = GetTextileSelection();
	}
	CTextile* pTextile = TEXGEN.GetTextile(TextileName);
	CTexGenRenderWindow* pRenderWindow = GetRenderWindow(TextileName);
	if (pTextile && pRenderWindow)
	{
/*		string DomainName = pTextile->GetAssignedDomain();
		if (!CTexGen::GetInstance().GetDomain(DomainName))
		{
			DomainName = "";
			pTextile->AssignDomain(DomainName);
		}*/
		CTexGenRenderer* pRenderer = pRenderWindow->GetRenderer();
		if (pRenderer)
		{
			if (pRenderer->GetNumProps(CTexGenRenderer::PROP_NODE))
			{
				pRenderer->RemoveNodes();
				pRenderer->RenderNodes(TextileName);
			}
			if (pRenderer->GetNumProps(CTexGenRenderer::PROP_PATH))
			{
				pRenderer->RemovePaths();
				pRenderer->RenderPaths(TextileName);
			}
			if (pRenderer->GetNumProps(CTexGenRenderer::PROP_SURFACE))
			{
				pRenderer->RemoveTextiles();
				pRenderer->RenderTextile(TextileName);
			}
			if (pRenderer->GetNumProps(CTexGenRenderer::PROP_VOLUME))
			{
				pRenderer->RemoveMeshes();
				pRenderer->RenderMesh(TextileName);
			}
			if (pRenderer->GetNumProps(CTexGenRenderer::PROP_INTERFERENCE))
			{
				pRenderer->RemoveInterference();
				pRenderer->RenderInterference(TextileName);
			}
			if ( pRenderer->GetNumProps(CTexGenRenderer::PROP_INTERFERENCE_DEPTH))
			{
				pRenderer->RemoveInterferenceDepth();
				pRenderer->RenderInterference( TextileName, true );
			}
			if (pRenderer->GetNumProps(CTexGenRenderer::PROP_ORIENTATION))
			{
				pRenderer->RemoveOrientations();
				pRenderer->RenderGrid(TextileName);
			}
			if (pRenderer->GetNumProps(CTexGenRenderer::PROP_DOMAIN))
			{
				pRenderer->RemoveDomains();
				pRenderer->RenderDomain(TextileName);
			}
			if (pRenderer->GetNumProps(CTexGenRenderer::PROP_DOMAINAXES))
			{
				pRenderer->RemoveDomainAxes();
				pRenderer->RenderDomainAxes(TextileName);
			}
			return true;
		}
	}
	return false;
}

string CTexGenMainFrame::GetUserSelectedColor()
{
	wxColourDialog ColorDialog(this);
	int i;
	for (i=0; i<16; ++i)
	{
		COLOR IndexedColor = GetIndexedColor(i);
		wxColour Color;
		Color.Set((unsigned char)(IndexedColor.r*255), (unsigned char)(IndexedColor.g*255), (unsigned char)(IndexedColor.b*255));
		ColorDialog.GetColourData().SetCustomColour(i, Color);
	}
	if (ColorDialog.ShowModal() == wxID_OK)
	{
		wxColour Color = ColorDialog.GetColourData().GetColour();
		double r, g, b;
		r = Color.Red()/255.0;
		g = Color.Green()/255.0;
		b = Color.Blue()/255.0;
		string ReturnVal = "COLOR(";
		ReturnVal += stringify(r) + ", ";
		ReturnVal += stringify(g) + ", ";
		ReturnVal += stringify(b) + ")";
		return ReturnVal;
	}
	return "";
}


string CTexGenMainFrame::GetTextileSelection()
{
	if (m_pViewerNotebook)
	{
		int iIndex = m_pViewerNotebook->GetSelection();
		if (iIndex != -1)
		{
			return ConvertString(m_pViewerNotebook->GetPageText(iIndex));
		}
	}
	return "";
/*	const map<string, CTextile*> &Textiles = CTexGen::GetInstance().GetTextiles();

	if (Textiles.empty())
	{
		wxMessageBox("The textile list is empty!", "No textiles", wxOK | wxICON_ERROR, this);
		return "";
	}

	wxArrayString TextileNames;
	map<string, CTextile*>::const_iterator itTextile;
	for (itTextile = Textiles.begin(); itTextile != Textiles.end(); ++itTextile)
	{
		TextileNames.Add(itTextile->first.c_str());
	}

	return wxGetSingleChoice("Select a textile from the list below", "Select textile", TextileNames, this).c_str();*/
}

void CTexGenMainFrame::ProcessFiles(const wxArrayString& filenames)
{
	int i;
	for (i=0; i<(int)filenames.GetCount(); ++i)
	{
		const wxString &Filename = filenames[i];
		if (Filename.AfterLast('.') == wxT("tg3"))
		{
			string Command;
			Command = "ReadFromXML(r\"";
			Command += ConvertString(Filename);
			Command += "\")";
			SendPythonCode(Command);
		}
		else if (Filename.AfterLast('.') == wxT("py"))
		{
			string Command;
			Command = "execfile(r\"";
			Command += ConvertString(Filename);
			Command += "\")";
			SendPythonCode(Command);
		}
	}
}


void CTexGenMainFrame::TextileCallback(string TextileName, bool bAdded)
{
	DeleteRenderWindow(TextileName);  //Need to delete window both if deleting textile or overwriting
	if (bAdded)
	{
		CTexGenRenderWindow* pRenderWindow = CreateRenderWindow(TextileName);  
		CTexGenRenderer* pRenderer;
		CTextile* pTextile = CTexGen::Instance().GetTextile(TextileName);
		if (pRenderWindow && (pRenderer = pRenderWindow->GetRenderer()) && pTextile)
		{
			pRenderer->RenderTextile(TextileName);
			pRenderer->RenderDomain(TextileName);
			pRenderer->RenderDomainAxes(TextileName);
			pRenderer->RenderAxes();
		}
	}
}

void CTexGenMainFrame::RendererCallback()
{
	UpdateRenderingPage();
}

void CNoteBookHandler::OnSetFocus(wxFocusEvent &event)
{
	CTexGenRenderWindow* pRenderWindow = m_MainFrame.GetRenderWindow();
	if (pRenderWindow)
		pRenderWindow->SetFocus();
/*	if (pRenderWindow)
		pRenderWindow->OnKeyDown(event);
	else
		event.Skip();*/
}

void CTexGenMainFrame::OnPaneButton(wxAuiManagerEvent& event)
{
	if (event.GetButton() == wxAUI_BUTTON_CLOSE)
	{
		event.GetPane()->Hide();
		UpdateWindowChecks();
		m_Manager.Update();
	}
	else
		event.Skip();
}

void COutlineHandler::OnSelectionChanged(wxTreeEvent& event)
{
	CTexGenRenderWindow* pRenderWindow = m_MainFrame.GetRenderWindow();
	if (pRenderWindow)
	{
		CModeller* pModeller = pRenderWindow->GetModeller();
		if (pModeller)
		{
			pModeller->OnOutlinerSelectionChanged(m_MainFrame.GetOutliner());
		}
	}
}

void COutlineHandler::OnDeleteSelected(wxCommandEvent& event)
{
	CTexGenRenderWindow* pRenderWindow = m_MainFrame.GetRenderWindow();
	if (pRenderWindow)
	{
		CModeller* pModeller = pRenderWindow->GetModeller();
		if (pModeller)
		{
			pModeller->DeleteSelectedObjects();
		}
	}
}

void COutlineHandler::OnInsertNode(wxCommandEvent& event)
{
	CTexGenRenderWindow* pRenderWindow = m_MainFrame.GetRenderWindow();
	if (pRenderWindow)
	{
		CModeller* pModeller = pRenderWindow->GetModeller();
		if (pModeller)
		{
			pModeller->InsertNodes();
		}
	}
}

void COutlineHandler::OnDuplicateYarn(wxCommandEvent& event)
{
	CTexGenRenderWindow* pRenderWindow = m_MainFrame.GetRenderWindow();
	if (pRenderWindow)
	{
		CModeller* pModeller = pRenderWindow->GetModeller();
		if (pModeller)
		{
			pModeller->DuplicateYarns();
		}
	}
}

void CTexGenMainFrame::OnOutputMessages(wxCommandEvent& event) {
	if (event.IsChecked()) {
		CTexGen::Instance().SetMessages(true, CLoggerGUI());
	}
	else {
		CTexGen::Instance().SetMessages(false, CLoggerNull());
	}
}

void CTexGenMainFrame::OnPatternDraft(wxCommandEvent& event) {
	string TextileName = GetTextileSelection();
	CTextile* pTextile = TEXGEN.GetTextile(TextileName);
	if (!pTextile)
	{
		wxMessageBox(wxT("Cannot create pattern draft - no textile loaded"), wxT("Pattern Draft Error"), wxOK | wxICON_ERROR, this);
		return;
	}
	string Type = pTextile->GetType();
	stringstream Command;

	// Get appropriate weave type
	if (Type == "CTextileWeave2D")
		Command << "Weave = GetTextile( '" << TextileName << "' ).GetWeave()" << endl;
	else if (Type == "CTextile3DWeave")
		Command << "Weave = GetTextile( '" << TextileName << "' ).Get3DWeave()" << endl;
	else if (Type == "CTextileOrthogonal")
		Command << "Weave = GetTextile( '" << TextileName << "' ).GetOrthogonalWeave()" << endl;
	else if (Type == "CTextileLayerToLayer")
		Command << "Weave = GetTextile( '" << TextileName << "' ).GetLayerToLayerWeave()" << endl;
	else
	{
		wxMessageBox(wxT("Cannot create pattern draft - incorrect weave type"), wxT("Pattern Draft Error"), wxOK | wxICON_ERROR, this);
		return;
	}

	Command << "Weave.ConvertToPatternDraft()" << endl;
	Command << "PatternDraft = Weave.GetPatternDraft()" << endl;
	Command << "PatternDraft.CreatePatternDraft()" << endl;

	SendPythonCode(Command.str());

	CPatternDraftDialog Dialog;
	if (Type == "CTextileWeave2D")
		Dialog.SetPatternDraft(((CTextileWeave2D*)pTextile)->GetPatternDraft());
	else if (Type == "CTextile3DWeave")
		Dialog.SetPatternDraft(((CTextile3DWeave*)pTextile)->GetPatternDraft());
	else if (Type == "CTextileOrthogonal")
		Dialog.SetPatternDraft(((CTextileOrthogonal*)pTextile)->GetPatternDraft());
	else
		Dialog.SetPatternDraft(((CTextileLayerToLayer*)pTextile)->GetPatternDraft());

	if (Dialog.ShowModal() == wxID_OK)
	{

	}
}

void CTexGenMainFrame::OnDomainVolumeFraction(wxCommandEvent& event)
{
	string TextileName = GetTextileSelection();
	CTextile* Textile = TEXGEN.GetTextile(TextileName);
	if (!Textile)
	{
		wxMessageBox(wxT("Cannot calculate volume fraction - no textile loaded"), wxT("Volume Fraction Error"), wxOK | wxICON_ERROR, this);
		return;
	}
	stringstream StringStream;
	double yarnVolume = Textile->GetYarnVolume();
	double yarnRealVolume = ConvertUnits(yarnVolume, Textile->GetGeometryScale() + "^3", "m^3");
	StringStream << "Volume yarns: " << yarnRealVolume << " m^3" << endl;

	vector<CYarn, allocator<CYarn>> yarns = Textile->GetYarns();
	StringStream << "Yarns: " << yarns.size() << endl;

	double weftVolume = 0;
	double warpVolume = 0;
	double binderVolume = 0;

	for (size_t i = 0; i < yarns.size(); i++) {
		double val = ConvertUnits(yarns[i].GetRawYarnVolume(), Textile->GetGeometryScale() + "^3", "m^3");
		switch (yarns[i].GetType())
		{
		case CYarn::WARP:
			StringStream << "Volume yarn warp   " << i << " : " << val << " m^3" << endl;
			warpVolume += val;
			break;
		case CYarn::WEFT:
			StringStream << "Volume yarn weft   " << i << " : " << val << " m^3" << endl;
			weftVolume += val;
			break;
		default:
			StringStream << "Volume yarn binder " << i << " : " << val << " m^3" << endl;
			binderVolume += val;
			break;
		}
	}

	if (yarnRealVolume > 0) {
		double volumeWeftPercent = weftVolume / yarnRealVolume * 100;
		StringStream << "Volume percent of weft    : " << volumeWeftPercent << " %" << endl;

		double volumeWarpPercent = warpVolume / yarnRealVolume * 100;
		StringStream << "Volume percent of warp   : " << volumeWarpPercent << " %" << endl;

		double volumeBinderPercent = binderVolume / yarnRealVolume * 100;
		StringStream << "Volume percent of binder: " << volumeBinderPercent << " %" << endl;
	}
	else {
		StringStream << "Yarn's volume percent: yarn volume not may by equals zero" << endl;
	}

	if (Textile->GetDomain() != NULL) {
		double domain = Textile->GetDomain()->GetVolume();
		double domainReal = ConvertUnits(domain, Textile->GetGeometryScale() + "^3", "m^3");
		StringStream << "Domain: " << domainReal << " m^3" << endl;
		if (domainReal > 0) {
			double volumePercent = yarnRealVolume / domainReal * 100;
			StringStream << "Volume percent: " << volumePercent << " %" << endl;
		}
		else {
			StringStream << "Volume percent: domain not may by equals zero" << endl;
		}
	}
	else {
		StringStream << "Domain: domain not exist" << endl;
	}
	//double VolumeFraction = Textile->GetDomainVolumeFraction();
	//wxString VfMessage = wxString::Format(wxT("Domain fibre volume fraction = %.6f"), VolumeFraction);





	wxMessageBox(ConvertString(StringStream.str()), wxT("Textile Volume"), wxOK | wxCENTRE, this);
}

void CTexGenMainFrame::OnQuickDomainVolumeFraction(wxCommandEvent& event) {
	string TextileName = GetTextileSelection();
	CTextile* pTextile = TEXGEN.GetTextile(TextileName);
	if (!pTextile)
	{
		wxMessageBox(wxT("Cannot calculate volume fraction - no textile loaded"), wxT("Volume Fraction Error"), wxOK | wxICON_ERROR, this);
		return;
	}

	if (!pTextile->GetDomain())
	{
		wxMessageBox(wxT("Cannot calculate volume fraction - no domain specified"), wxT("Volume Fraction Error"), wxOK | wxICON_ERROR, this);
		return;
	}

	double VolumeFraction = pTextile->GetQuickDomainVolumeFraction();
	wxString VfMessage = wxString::Format(wxT("Domain fibre volume fraction = %.6f\nCalculated using fibre volume fraction for whole yarns\nUse Calculate Domain Volume Fraction to use localised Vf where yarn cross-section varies"), VolumeFraction);
	wxMessageBox(VfMessage, wxT("Volume Fraction"), wxOK | wxCENTRE, this);
}

CAbaqusVoxelInput::CAbaqusVoxelInput(wxWindow* parent)
{
	wxXmlResource::Get()->LoadDialog(this, parent, wxT("AbaqusVoxelOptions"));
}

void CAbaqusVoxelInput::OnOffsetUpdate(wxUpdateUIEvent& event)
{
	wxRadioBox* PeriodicBoundariesCtrl = (wxRadioBox*)FindWindow(XRCID("PeriodicBoundaries"));
	if ( PeriodicBoundariesCtrl->GetSelection() == STAGGERED_BC )
	{
		event.Enable(true);
	}
	else
	{
		event.Enable(false);
	}
}

CVolumeMeshOptions::CVolumeMeshOptions(wxWindow* parent)
{
	wxXmlResource::Get()->LoadDialog(this, parent, wxT("VolumeMeshOptions"));
}

void CVolumeMeshOptions::OnPeriodicBoundariesUpdate(wxUpdateUIEvent& event)
{
	wxCheckBox* PeriodicCtrl = (wxCheckBox*)FindWindow(XRCID("Periodic"));
	if ( PeriodicCtrl->GetValue() )
	{
		event.Enable(true);
		wxRadioBox* PeriodicBoundariesCtrl = (wxRadioBox*)FindWindow(XRCID("PeriodicBoundaries"));
		PeriodicBoundariesCtrl->Enable(STAGGERED_BC, false);
	}
	else
	{
		event.Enable(false);
	}
}

CSurveyDialog::CSurveyDialog(wxWindow* parent)
{
	wxXmlResource::Get()->LoadDialog(this, parent, wxT("Survey"));
}

void CSurveyDialog::OnClickNextTime( wxCommandEvent& event )
{
	this->EndModal(wxID_CANCEL);
}

void CSurveyDialog::OnClickNotAgain( wxCommandEvent& event )
{
	int count = -1;
	ofstream countFile(m_Filename.c_str());
	countFile << count;
	countFile.close();

	this->EndModal(wxID_CANCEL);
}

void CSurveyDialog::OnClickTakeSurvey( wxHyperlinkEvent& event )
{
	int count = 1;
	ofstream countFile(m_Filename.c_str());
	countFile << count;
	countFile.close();
	wxString url = event.GetURL();
	if (!wxLaunchDefaultBrowser(url))
	{
		wxLogWarning(wxT("Could not launch the default browser with url '%s' !"), url.c_str());
	}
	this->EndModal(wxID_CANCEL);
}
