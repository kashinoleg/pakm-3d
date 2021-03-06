﻿#include "TexGenMainFrame.h"
#include "LoggerGUI.h"
#include "Exporter.h"

#define EVT_BUTTON_MENU_RANGE(id1, id2, func) EVT_COMMAND_RANGE(id1, id2, wxEVT_COMMAND_BUTTON_CLICKED, func) EVT_COMMAND_RANGE(id1, id2, wxEVT_COMMAND_MENU_SELECTED, func)
#define EVT_CHECKBOX_MENU_RANGE(id1, id2, func) EVT_COMMAND_RANGE(id1, id2, wxEVT_COMMAND_CHECKBOX_CLICKED, func) EVT_COMMAND_RANGE(id1, id2, wxEVT_COMMAND_MENU_SELECTED, func)
#define EVT_RADIOBUTTON_MENU_RANGE(id1, id2, func) EVT_COMMAND_RANGE(id1, id2, wxEVT_COMMAND_RADIOBUTTON_SELECTED, func) EVT_COMMAND_RANGE(id1, id2, wxEVT_COMMAND_MENU_SELECTED, func)


BEGIN_EVENT_TABLE(CTexGenMainFrame, wxFrame)
EVT_MENU(ID_Quit, CTexGenMainFrame::OnQuit)
EVT_MENU(ID_About, CTexGenMainFrame::OnAbout)
EVT_MENU(ID_Open, CTexGenMainFrame::OnOpen)
EVT_MENU(ID_Save, CTexGenMainFrame::OnSave)
EVT_MENU(ID_SaveScreenshot, CTexGenMainFrame::OnSaveScreenshot)
EVT_MENU(ID_SaveVolumeMesh, CTexGenMainFrame::OnSaveVolumeMesh)
EVT_MENU(ID_SaveSurfaceMesh, CTexGenMainFrame::OnSaveSurfaceMesh)
EVT_MENU(ID_SaveIGES, CTexGenMainFrame::OnSaveIGES)
EVT_MENU(ID_SaveSTL, CTexGenMainFrame::OnSaveSTL)
EVT_MENU(ID_SaveSTEP, CTexGenMainFrame::OnSaveSTEP)
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
;


CTexGenMainFrame::CTexGenMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
, m_pViewerNotebook(NULL)
, m_pLogNotebook(NULL)
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
		wxMenu *pExportSubMenu = new wxMenu;
		pExportSubMenu->Append(ID_SaveScreenshot, wxT("&Image (*.png)"));
		pExportSubMenu->Append(ID_SaveIGES, wxT("&IGES (*.igs)"));
		pExportSubMenu->Append(ID_SaveSTEP, wxT("&STEP (*.stp)"));
		pExportSubMenu->Append(ID_SaveSTL, wxT("&STL (*.stl)"));


		pExportSubMenu->Append(ID_SaveSurfaceMesh, wxT("Sur&face Mesh..."));
		{
			wxMenu *pAbaqusSubMenu = new wxMenu;
			pAbaqusSubMenu->Append(ID_SaveVolumeMesh, wxT("&Volume Mesh..."));
			pAbaqusSubMenu->Append(ID_SaveTetgenMesh, wxT("&TetgenMesh..."));
			pAbaqusSubMenu->Append(ID_SaveABAQUSVoxels, wxT("&Voxel File..."));
			pExportSubMenu->Append(wxID_ANY, wxT("&ABAQUS (*.inp)"), pAbaqusSubMenu);
		}
		pMenuFile->Append(wxID_ANY, wxT("&Export..."), pExportSubMenu);
	}
	pMenuFile->AppendSeparator();
	pMenuFile->Append(ID_About, wxT("&About..."));
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
		m_pTexGenOutput = new wxTextCtrl(m_pLogNotebook, wxID_ANY, wxEmptyString,
			wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_DONTWRAP | wxTE_RICH);
		m_pTexGenOutput->SetFont(ConsoleFont);
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

void CTexGenMainFrame::ReceiveOutput(string Text, bool bError)
{
	auto pOutputWindow = m_pTexGenOutput;
	auto PrevStyle = pOutputWindow->GetDefaultStyle();
	if (bError)
	{
		pOutputWindow->SetDefaultStyle(wxTextAttr(*wxRED));
	}
	pOutputWindow->AppendText(ConvertString(Text));
	pOutputWindow->SetDefaultStyle(PrevStyle);
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
			auto renderer = GetRendererWindow();
			if (dialog.ShowModal() == wxID_OK && renderer)
			{
				auto MagCtrl = (wxSpinCtrl*)FindWindow(XRCID("Magnification"));
				renderer->TakeScreenShot(ConvertString(dialog.GetPath()), MagCtrl->GetValue());
			}
		}
	}
}

void CTexGenMainFrame::OnSaveVolumeMesh(wxCommandEvent& event)
{
	string TextileName = GetTextileSelection();
	auto pTextile = CTexGen::Instance().GetTextile(TextileName);

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
				CMesher* mesher = nullptr;
				if (bPeriodic)
				{
					mesher = new CMesher(iBoundaryConditions);
				}
				else
				{
					mesher = new CMesher();
				}
				if (iElementOrder == 1)
				{
					mesher->SetQuadratic(true);
				}
				if (!bProjectMidSideNodes)
				{
					mesher->SetProjectMidSideNodes(false);
				}
				if (bPeriodic)
				{
					mesher->SetPeriodic(true);
				}
				if (!SeedSize.IsEmpty())
				{
					mesher->SetSeed(Convert::ToDouble(SeedSize));
				}
				if (!MergeTolerance.IsEmpty())
				{
					mesher->SetMergeTolerance(Convert::ToDouble(MergeTolerance));
				}
				mesher->CreateMesh(TextileName);
				switch (dialog.GetFilterIndex())
				{
				case 0:
					mesher->SaveVolumeMeshToABAQUS(Convert::ToString(dialog.GetPath()), TextileName);
					break;
				case 1:
					mesher->SaveVolumeMeshToVTK(Convert::ToString(dialog.GetPath()));
					break;
				}
			}
		}
	}
}

void CTexGenMainFrame::OnSaveSTL(wxCommandEvent& event)
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
				wxT("Save STL file"),
				wxGetCwd(),
				wxEmptyString,
				wxT("ASCII STL file (*.stl)|*.stl|")
				wxT("Binary STL file (*.stl;*stlb)|*.stl;*stlb|"),
				wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR
			);
			dialog.CentreOnParent();
			if (dialog.ShowModal() == wxID_OK)
			{
				auto mesh = CMesh();
				auto textile = CTexGen::Instance().GetTextile(TextileName);
				textile->AddSurfaceToMesh(mesh, bTrimSurface);
				if (bExportDomain)
				{
					mesh.InsertMesh(textile->GetDomain()->GetMesh());
				}
				if (dialog.GetFilterIndex() == 0)
				{
					mesh.SaveToSTLAscii(ConvertString(dialog.GetPath()));
				}
				else
				{
					mesh.SaveToSTLBinary(ConvertString(dialog.GetPath()));
				}
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
				auto mesh = CMesh();
				auto textile = CTexGen::Instance().GetTextile(TextileName);
				textile->AddSurfaceToMesh(mesh, bTrimSurface);
				if (bExportDomain)
				{
					mesh.InsertMesh(textile->GetDomain()->GetMesh());
				}
				switch (dialog.GetFilterIndex())
				{
				case 0:
					mesh.SaveToVTK(ConvertString(dialog.GetPath()));
					break;
				case 1:
					mesh.SaveToSCIRun(ConvertString(dialog.GetPath()));
					break;
				}
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
				auto path = ConvertString(dialog.GetPath());
				auto textileName = GetTextileSelection();
				CExporter::NameTextileToIGES(path, textileName, bExportDomain, bSubtractYarns);
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
				auto path = ConvertString(dialog.GetPath());
				auto textileName = GetTextileSelection();
				CExporter::NameTextileToSTEP(path, textileName, bExportDomain, bSubtractYarns);
			}
		}
	}
}

void CTexGenMainFrame::OnSaveABAQUSVoxels(wxCommandEvent& event)
{
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
				string TextileName = GetTextileSelection();
				auto textile = CTexGen::Instance().GetTextile(TextileName);
				auto outputFilename = ConvertString(dialog.GetPath());
				auto XVoxNum = Convert::ToInt(XVoxels);
				auto YVoxNum = Convert::ToInt(YVoxels);
				auto ZVoxNum = Convert::ToInt(ZVoxels);

				if ( iBoundaryConditions == SHEARED_BC )
				{
					auto vox = new CShearedVoxelMesh("CShearedPeriodicBoundaries");
					vox->SaveVoxelMesh(*textile, outputFilename, XVoxNum, YVoxNum, ZVoxNum, bOutputMatrix, bOutputYarns, iBoundaryConditions, iElementType);
				}
				else if ( iBoundaryConditions == STAGGERED_BC )
				{
					auto vox = new CStaggeredVoxelMesh("CStaggeredPeriodicBoundaries");
					auto offset = Convert::ToDouble(XOffset);
					vox->SetOffset(offset);
					vox->SaveVoxelMesh(*textile, outputFilename, XVoxNum, YVoxNum, ZVoxNum, bOutputMatrix, bOutputYarns, iBoundaryConditions, iElementType);
				}
				else
				{
					auto vox = new CRectangularVoxelMesh("CPeriodicBoundaries");
					vox->SaveVoxelMesh(*textile, outputFilename, XVoxNum, YVoxNum, ZVoxNum, bOutputMatrix, bOutputYarns, iBoundaryConditions, iElementType);
				}
			}
		}
	}
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

CTexGenRenderer* CTexGenMainFrame::GetRendererWindow(string WindowName)
{
	if (wxTheApp)
	{
		CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
		if (pMainFrame)
		{
			CTexGenRenderWindow* pRenderWindow = pMainFrame->GetRenderWindow(WindowName);
			if (pRenderWindow)
			{
				return pRenderWindow->GetRenderer();
			}
		}
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
		OnEditTextile(); break;
	case ID_CreateEmptyTextile:
		OnCreateEmptyTextile(); break;
	case ID_Create2DWeave:
		OnCreate2DWeave(); break;
	case ID_GeometrySolve:
		OnGeometrySolve(event); break;
	case ID_Create3DTextile:
		OnCreate3DTextile(); break;
	case ID_CreateLayeredTextile:
		OnCreateLayeredTextile(); break;
	case ID_SetLayerOffsets:
		OnSetLayerOffsets(); break;
	case ID_NestLayers:
		OnNestLayers(); break;
	case ID_MaxNestLayers:
		OnMaxNestLayers(); break;
	}
}

void CTexGenMainFrame::OnEditTextile()
{
	string TextileName = GetTextileSelection();
	if (!TextileName.empty())
	{
		auto textile = CTexGen::Instance().GetTextile(TextileName);
		string Type = textile->GetType();
		if (textile && (Type == "CTextileWeave2D" || Type == "CShearedTextileWeave2D" ||
			Type == "CTextileOrthogonal" || Type == "CTextileAngleInterlock" || Type == "CTextileOffsetAngleInterlock" || Type == "CTextileLayerToLayer"))
		{
			if (Type == "CTextileWeave2D" || Type == "CShearedTextileWeave2D")
			{
				CWeaveWizard Wizard(this, wxID_ANY);
				if (Type == "CTextileWeave2D")
				{
					Wizard.LoadSettings(*((CTextileWeave2D*)textile));
				}
				else
				{
					Wizard.LoadSettings(*((CShearedTextileWeave2D*)textile));
				}
				if (Wizard.RunIt())
				{
					Wizard.GetCreateTextile(TextileName);
				}
			}
			else // if (pTextile->GetType() == "CTextileWeave3D")
			{
				CWeaveWizard3D Wizard(this, wxID_ANY);
				Wizard.LoadSettings(*((CTextile3DWeave*)textile));
				if (Wizard.RunIt())
				{
					Wizard.CreateTextile(TextileName);
				}
			}
		}
	}
}

void CTexGenMainFrame::OnCreate2DWeave()
{
	CWeaveWizard Wizard(this, wxID_ANY);
	if (Wizard.RunIt())
	{
		Wizard.GetCreateTextile();
	}
}

void CTexGenMainFrame::OnCreate3DTextile()
{
	CWeaveWizard3D Wizard(this, wxID_ANY);
	if (Wizard.RunIt())
	{
		Wizard.CreateTextile();
	}
}

void CTexGenMainFrame::OnCreateLayeredTextile()
{
	CTextileLayersDialog Dialog;
	if (Dialog.ShowModal() == wxID_OK)
	{
		vector<string> LayerNames;
		Dialog.GetLayerNames(LayerNames);
		if (!LayerNames.empty())
		{
			auto LayeredTextile = new CTextileLayered();
			XYZ Offset;
			XYZ Min;
			XYZ Max;
			for (vector<string>::reverse_iterator itLayerNames = LayerNames.rbegin(); itLayerNames != LayerNames.rend(); itLayerNames++)
			{
				auto textile = new CTextile(*CTexGen::Instance().GetTextile(*itLayerNames));
				CDomain* pDomain = textile->GetDomain();
				pair<XYZ, XYZ> AABB;
				if (!pDomain)
				{
					string str = "Cannot output textile, " + (*itLayerNames) + ". No domain specified ";
					TGERROR(str);
					continue;
				}
				else
				{
					AABB = pDomain->GetMesh().GetAABB();
				}

				if (itLayerNames != LayerNames.rbegin())
				{
					Offset.z -= AABB.first.z;
					Min.x = Min.x < AABB.first.x ? Min.x : AABB.first.x;
					Min.y = Min.y < AABB.first.y ? Min.y : AABB.first.y;
					Max.x = Max.x > AABB.second.x ? Max.x : AABB.second.x;
					Max.y = Max.y > AABB.second.y ? Max.y : AABB.second.y;
				}
				else
				{
					Min = AABB.first;
					Max = AABB.second;
				}
				auto offset = XYZ(Offset);
				LayeredTextile->AddLayer(*textile, offset);
				Offset.z += AABB.second.z;
			}

			Max.z = Offset.z;
			auto domain = new CDomainPlanes(XYZ(Min), XYZ(Max));
			LayeredTextile->AssignDomain(*domain);
			auto offset = XY(0, 0);
			LayeredTextile->SetOffsets(offset);
			CTexGen::Instance().AddTextile(*LayeredTextile);
		}
	}
}

void CTexGenMainFrame::OnMaxNestLayers()
{
	string TextileName = GetTextileSelection();
	if (!TextileName.empty())
	{
		CTextile* pTextile = CTexGen::Instance().GetTextile(TextileName);
		string Type = pTextile->GetType();
		if (Type != "CTextileLayered")
		{
			TGERROR("Cannot nest layers: not a layered textile");
			return;
		}
		auto textile = CTexGen::Instance().GetTextile(TextileName);
		textile->GetLayeredTextile()->MaxNestLayers();
		RefreshTextile(TextileName);
	}
}

void CTexGenMainFrame::OnNestLayers()
{
	string TextileName = GetTextileSelection();
	if (!TextileName.empty())
	{
		CTextile* pTextile = CTexGen::Instance().GetTextile(TextileName);
		string Type = pTextile->GetType();
		if (Type != "CTextileLayered")
		{
			TGERROR("Cannot nest layers: not a layered textile");
			return;
		}
		auto textile = CTexGen::Instance().GetTextile(TextileName);
		textile->GetLayeredTextile()->NestLayers();
		RefreshTextile(TextileName);
	}
}

void CTexGenMainFrame::OnSetLayerOffsets()
{
	string TextileName = GetTextileSelection();
	if (!TextileName.empty())
	{
		CTextile* pTextile = CTexGen::Instance().GetTextile(TextileName);
		string Type = pTextile->GetType();
		if (Type != "CTextileLayered")
		{
			TGERROR("Cannot set layers: not a layered textile");
			return;
		}

		CLayersOffsetDialog Dialog;
		XYZ Min, Max;
		CDomain* Domain = pTextile->GetDomain();
		((CDomainPlanes*)Domain)->GetBoxLimits(Min, Max);
		XY DomainSize(Max.x - Min.x, Max.y - Min.y);
		Dialog.LoadSettings(dynamic_cast<CTextileLayered*>(pTextile)->GetOffsets(), DomainSize);

		if (Dialog.ShowModal() == wxID_OK)
		{
			switch (Dialog.GetOption())
			{
			case CONSTANT:
			{
				auto textile = CTexGen::Instance().GetTextile(TextileName);
				auto layeredTextile = textile->GetLayeredTextile();
				auto Offset = Dialog.GetConstantOffset();
				layeredTextile->SetOffsets(Offset);
				break;
			}
			case RANDOM:
			case EDIT:
				auto LayerOffsets = Dialog.GetEditOffsets();
				auto textile = CTexGen::Instance().GetTextile(TextileName);
				auto layeredTextile = textile->GetLayeredTextile();
				layeredTextile->SetOffsets(LayerOffsets);
				break;
			}
			RefreshTextile(TextileName);
		}
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
			auto solver = CGeometrySolver();
			if (!SeedSize.IsEmpty())
			{
				solver.SetSeed(Convert::ToDouble(SeedSize));
			}
			if (!TensileStress.IsEmpty())
			{
				solver.SetTensileStress(Convert::ToDouble(TensileStress));
			}
			if (!LongitudinalBendingModulus.IsEmpty())
			{
				solver.SetLongitudinalBendingModulus(Convert::ToDouble(LongitudinalBendingModulus));
			}
			if (!TransverseBendingModulus.IsEmpty())
			{
				solver.SetTransverseBendingModulus(Convert::ToDouble(TransverseBendingModulus));
			}
			if (!ContactStiffness.IsEmpty())
			{
				solver.SetContactStiffness(Convert::ToDouble(ContactStiffness));
			}
			if (!DisabledStiffness.IsEmpty())
			{
				solver.SetDisabledStiffness(Convert::ToDouble(DisabledStiffness));
			}
			solver.CreateSystem(TextileName);
			solver.SolveSystem();
			if (bCreateCopy)
			{
				solver.GetDeformedCopyOfTextile();
			}
			else
			{
				solver.DeformTextile();
			}
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

void CTexGenMainFrame::OnDomains(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	case ID_DeleteDomain:
		OnDeleteDomain(); break;
	case ID_EditDomain:
		OnEditDomain(); break;
	case ID_CreateDomainPlanes:
		OnCreateDomainPlanes(); break;
	case ID_CreateDomainBox:
		OnCreateDomainBox();break;
	}
}

void CTexGenMainFrame::OnDeleteDomain()
{
	string TextileName = GetTextileSelection();
	if (!TextileName.empty())
	{
		auto textile = CTexGen::Instance().GetTextile(TextileName);
		if (textile != NULL)
		{
			textile->RemoveDomain();
			RefreshTextile(TextileName);
		}
	}
}

void CTexGenMainFrame::OnEditDomain()
{
	string TextileName = GetTextileSelection();
	if (!TextileName.empty())
	{
		auto textile = CTexGen::Instance().GetTextile(TextileName);
		if (textile)
		{
			const CDomain* pDomain = textile->GetDomain();
			if (pDomain && pDomain->GetType() == "CDomainPlanes")
			{
				CDomainPlanesDialog Dialog(this, wxID_ANY);
				Dialog.LoadSettings(*((CDomainPlanes*)pDomain));
				if (Dialog.ShowModal() == wxID_OK)
				{
					Dialog.CreateDomain();
					RefreshTextile(TextileName);
				}
			}
		}
	}
}

void CTexGenMainFrame::OnCreateDomainPlanes()
{
	CDomainPlanesDialog Dialog(this, wxID_ANY);
	if (Dialog.ShowModal() == wxID_OK)
	{
		string TextileName = GetTextileSelection();
		if (!TextileName.empty())
		{
			Dialog.CreateDomain();
			RefreshTextile(TextileName);
		}
	}
}

void CTexGenMainFrame::OnCreateDomainBox()
{
	CDomainPlanesDialog Dialog(this, wxID_ANY);
	if (Dialog.CreateBox())
	{
		string TextileName = GetTextileSelection();
		if (!TextileName.empty())
		{
			Dialog.CreateDomain();
			RefreshTextile(TextileName);
		}
	}
}

void CTexGenMainFrame::OnRendering(wxCommandEvent& event)
{
	string TextileName = GetTextileSelection();
	if (TextileName.empty())
	{
		return;
	}
	auto pTextile = CTexGen::Instance().GetTextile(TextileName);
	if (!pTextile)
	{
		return;
	}
	switch (event.GetId())
	{
	case ID_RenderNodes:
		if (event.IsChecked())
		{
			GetRendererWindow(TextileName)->RenderNodes(TextileName);
		}
		else
		{
			GetRendererWindow(TextileName)->RemoveNodes();
		}
		break;
	case ID_RenderPaths:
		if (event.IsChecked())
		{
			GetRendererWindow(TextileName)->RenderPaths(TextileName);
		}
		else
		{
			GetRendererWindow(TextileName)->RemovePaths();
		}
		break;
	case ID_RenderSurface:
		if (event.IsChecked())
		{
			GetRendererWindow(TextileName)->RenderTextile(TextileName);
		}
		else
		{
			GetRendererWindow(TextileName)->RemoveTextiles();
		}
		break;
	case ID_RenderVolume:
		if (event.IsChecked())
		{
			GetRendererWindow(TextileName)->RenderMesh(TextileName);
		}
		else
		{
			GetRendererWindow(TextileName)->RemoveMeshes();
		}
		break;
	case ID_RenderInterference:
		if (event.IsChecked())
		{
			GetRendererWindow(TextileName)->RenderInterference(TextileName);
		}
		else
		{
			GetRendererWindow(TextileName)->RemoveInterference();
		}
		break;
	case ID_RenderInterferenceDepth:
		if (event.IsChecked())
		{
			GetRendererWindow(TextileName)->RenderInterference(TextileName, true);
		}
		else
		{
			GetRendererWindow(TextileName)->RemoveInterferenceDepth();
		}
		break;
	case ID_RenderOrientation:
		if (event.IsChecked())
		{
			GetRendererWindow(TextileName)->RenderGrid(TextileName);
		}
		else
		{
			GetRendererWindow(TextileName)->RemoveOrientations();
		}
		break;
	case ID_RenderDomain:
		if (event.IsChecked())
		{
			GetRendererWindow(TextileName)->RenderDomain(TextileName);
		}
		else
		{
			GetRendererWindow(TextileName)->RemoveDomains();
		}
		break;
	case ID_RenderDomainAxes:
		if (event.IsChecked())
		{
			GetRendererWindow(TextileName)->RenderDomainAxes(TextileName);
		}
		else
		{
			GetRendererWindow(TextileName)->RemoveDomainAxes();
		}
		break;
	case ID_RenderAxes:
		if (event.IsChecked())
		{
			GetRendererWindow(TextileName)->RenderAxes();
		}
		else
		{
			GetRendererWindow(TextileName)->RemoveAxes();
		}
		break;
	case ID_XRay:
		{
			GetRendererWindow(TextileName)->SetXRay(event.IsChecked());
			auto rendWindow = GetRenderWindow(TextileName);
			if (rendWindow && rendWindow->GetRenderer() && rendWindow->GetRenderer()->GetNumProps(CTexGenRenderer::PROP_SURFACE))
			{
				GetRendererWindow(TextileName)->RemoveTextiles();
				GetRendererWindow(TextileName)->RenderTextile(TextileName);
			}
		}
		break;
	case ID_ParallelProjection:
		GetRendererWindow(TextileName)->SetParallelProjection(event.IsChecked()); break;
	case ID_TrimtoDomain:
		OnTrimtoDomain(event.IsChecked()); break;
	case ID_ChangeBackgroundColor:
		OnChangeBackgroundColor(); break;
	case ID_ChangeSurfaceColor:
		OnChangeSurfaceColor(); break;
	}
	UpdateRenderingPage();
}

void CTexGenMainFrame::OnTrimtoDomain(bool check)
{
	string TextileName = GetTextileSelection();
	if (!TextileName.empty())
	{
		GetRendererWindow(TextileName)->SetTrimToDomain(check);
		CTexGenRenderWindow* pRenderWindow = GetRenderWindow(TextileName);
		if (pRenderWindow)
		{
			CTexGenRenderer* pRenderer = pRenderWindow->GetRenderer();
			if (pRenderer)
			{
				if (pRenderer->GetNumProps(CTexGenRenderer::PROP_SURFACE))
				{
					GetRendererWindow(TextileName)->RemoveTextiles();
					GetRendererWindow(TextileName)->RenderTextile(TextileName);
				}
				if (pRenderer->GetNumProps(CTexGenRenderer::PROP_INTERFERENCE))
				{
					GetRendererWindow(TextileName)->RemoveInterference();
					GetRendererWindow(TextileName)->RenderInterference(TextileName);
				}
				if (pRenderer->GetNumProps(CTexGenRenderer::PROP_INTERFERENCE_DEPTH))
				{
					GetRendererWindow(TextileName)->RemoveInterferenceDepth();
					GetRendererWindow(TextileName)->RenderInterference(TextileName, true);
				}
				if (pRenderer->GetNumProps(CTexGenRenderer::PROP_ORIENTATION))
				{
					GetRendererWindow(TextileName)->RemoveOrientations();
					GetRendererWindow(TextileName)->RenderGrid(TextileName);
				}
			}
		}
	}
}

void CTexGenMainFrame::OnChangeBackgroundColor()
{
	string TextileName = GetTextileSelection();
	if (!TextileName.empty())
	{
		auto Color = GetUserSelectedColor();
		if (!Color.empty())
		{
			GetRendererWindow(TextileName)->SetBackgroundColor(Color);
		}
	}
}

void CTexGenMainFrame::OnChangeSurfaceColor()
{
	string TextileName = GetTextileSelection();
	if (!TextileName.empty())
	{
		auto Color = GetUserSelectedColor();
		if (!Color.empty())
		{
			GetRendererWindow(TextileName)->SetPropsColor(CTexGenRenderer::PROP_SURFACE, Color);
		}
	}
}

void CTexGenMainFrame::OnLogNotebook(wxAuiNotebookEvent& event)
{
	if (event.GetSelection() != -1)
	{
		m_pLogNotebook->SetPageBitmap(event.GetSelection(), wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_OTHER, wxSize(16, 16)));
	}
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
	{
		return;
	}
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

bool CTexGenMainFrame::RefreshTextile(string TextileName)
{
	if (TextileName.empty())
	{
		TextileName = GetTextileSelection();
	}
	auto pTextile = CTexGen::Instance().GetTextile(TextileName);
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

COLOR CTexGenMainFrame::GetUserSelectedColor()
{
	wxColourDialog ColorDialog(this);
	for (int i = 0; i < 16; i++)
	{
		COLOR IndexedColor = GetIndexedColor(i);
		wxColour Color;
		Color.Set((unsigned char)(IndexedColor.r*255), (unsigned char)(IndexedColor.g*255), (unsigned char)(IndexedColor.b*255));
		ColorDialog.GetColourData().SetCustomColour(i, Color);
	}
	if (ColorDialog.ShowModal() == wxID_OK)
	{
		wxColour Color = ColorDialog.GetColourData().GetColour();
		double r = Color.Red()/255.0;
		double g = Color.Green()/255.0;
		double b = Color.Blue()/255.0;
		return COLOR(r, g, b);
	}
	return COLOR::Empty();
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
	for (size_t i = 0; i < filenames.GetCount(); i++)
	{
		const wxString &Filename = filenames[i];
		if (Filename.AfterLast('.') == wxT("tg3"))
		{
			CTexGen::Instance().ReadFromXML(ConvertString(Filename));
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
	if (event.IsChecked())
	{
		CTexGen::Instance().SetMessages(true, CLoggerGUI());
	}
	else
	{
		CTexGen::Instance().SetMessages(false, CLoggerNull());
	}
}

void CTexGenMainFrame::OnPatternDraft(wxCommandEvent& event)
{
	string TextileName = GetTextileSelection();
	CTextile* pTextile = TEXGEN.GetTextile(TextileName);
	if (!pTextile)
	{
		wxMessageBox(wxT("Cannot create pattern draft - no textile loaded"), wxT("Pattern Draft Error"), wxOK | wxICON_ERROR, this);
		return;
	}
	string Type = pTextile->GetType();
	CPatternDraftDialog Dialog;
	if (Type == "CTextileWeave2D")
	{
		auto Weave = CTexGen::Instance().GetTextile(TextileName)->GetWeave();
		Weave->ConvertToPatternDraft();
		auto PatternDraft = Weave->GetPatternDraft();
		PatternDraft.CreatePatternDraft();
		Dialog.SetPatternDraft(PatternDraft);
	}
	else if (Type == "CTextile3DWeave")
	{
		auto Weave = CTexGen::Instance().GetTextile(TextileName)->Get3DWeave();
		Weave->ConvertToPatternDraft();
		auto PatternDraft = Weave->GetPatternDraft();
		PatternDraft.CreatePatternDraft();
		Dialog.SetPatternDraft(PatternDraft);
	}
	else if (Type == "CTextileOrthogonal")
	{
		auto Weave = CTexGen::Instance().GetTextile(TextileName)->GetOrthogonalWeave();
		Weave->ConvertToPatternDraft();
		auto PatternDraft = Weave->GetPatternDraft();
		PatternDraft.CreatePatternDraft();
		Dialog.SetPatternDraft(PatternDraft);
	}
	else if (Type == "CTextileLayerToLayer")
	{
		auto Weave = CTexGen::Instance().GetTextile(TextileName)->GetOrthogonalWeave();
		Weave->ConvertToPatternDraft();
		auto PatternDraft = Weave->GetPatternDraft();
		PatternDraft.CreatePatternDraft();
		Dialog.SetPatternDraft(PatternDraft);
	}
	else
	{
		wxMessageBox(wxT("Cannot create pattern draft - incorrect weave type"), wxT("Pattern Draft Error"), wxOK | wxICON_ERROR, this);
		return;
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
