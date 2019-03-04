#pragma once
#include <wx/colordlg.h>
#include <wx/xrc/xmlres.h>
#include <wx/aui/auibook.h>
#include <wx/printdlg.h>
#include <wx/hyperlink.h>
#include "TexGenRenderWindow.h"
#include "ControlsWindow.h"
#include "WindowIDs.h"
#include "WeaveWizard.h"
#include "DomainPlanesDialog.h"
#include "DropTarget.h"
#include "TexGenApp.h"
#include "CustomWidget.h"

#include "WeaveWizard3D.h"
#include "RangeValidator.h"
#include "TextileLayersDialog.h"
#include "LayersOffsetDialog.h"
#include "PatternDraftDialog.h"
#include "ConvertString.h"

class CTexGenMainFrame : public wxFrame
{
public:
	CTexGenMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~CTexGenMainFrame(void);

	void OnInit() {}

	void ReceiveOutput(string Text, bool bError);

	/// Shortcut function to get an instance of CTexGenRenderer associated with a particular window (if NULL the current)
	CTexGenRenderer *GetRendererWindow(string WindowName = "");

	CTexGenRenderWindow *GetRenderWindow(string WindowName = "");



	CTexGenRenderWindow *CreateRenderWindow(string WindowName = "");
	bool DeleteRenderWindow(string WindowName = "");
	void TextileCallback(string TextileName, bool bAdded);
	void RendererCallback();
	void ProcessFiles(const wxArrayString& filenames);
	void UpdatePositionText();

	bool RefreshTextile(string TextileName);

	wxPanel* GetOutliner() { return m_pOutliner; }

protected:
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);

	// File drop-down menu
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnSaveScreenshot(wxCommandEvent& event);
	void OnSaveVolumeMesh(wxCommandEvent& event);
	void OnSaveSurfaceMesh(wxCommandEvent& event);


	void OnSaveSTL(wxCommandEvent& event);

	void OnSaveIGES(wxCommandEvent& event);
	void OnSaveSTEP(wxCommandEvent& event);
	void OnSaveABAQUSVoxels(wxCommandEvent& event);
	void OnSaveTetgenMesh(wxCommandEvent& event);

	void OnWindow(wxCommandEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnRendering(wxCommandEvent& event);

	void OnTrimtoDomain(bool check);
	void OnChangeBackgroundColor();
	void OnChangeSurfaceColor();



	void OnTextiles(wxCommandEvent& event);

	void OnSetLayerOffsets();
	void OnNestLayers();
	void OnMaxNestLayers();
	void OnCreate3DTextile();
	void OnCreate2DWeave();
	void OnEditTextile();
	void OnCreateLayeredTextile();
	void OnCreateEmptyTextile();
	void OnDeleteTextile();


	void OnGeometrySolve(wxCommandEvent& event);
	void OnModeller(wxCommandEvent& event);



	void OnDomains(wxCommandEvent& event);
	void OnDeleteDomain();
	void OnEditDomain();
	void OnCreateDomainPlanes();
	void OnCreateDomainBox();
	
	


    void OnLogNotebook(wxAuiNotebookEvent& event);
    void OnViewerNotebookPageChanged(wxAuiNotebookEvent& event);
    void OnViewerNotebookClose(wxAuiNotebookEvent& event);
	void UpdateWindowChecks();
	void OnPaneButton(wxAuiManagerEvent& event);

	void OnPosition(wxCommandEvent& event);
	void OnSnapSize(wxCommandEvent& event);

	
	//
	void OnOutputMessages(wxCommandEvent& event);
	void OnPatternDraft(wxCommandEvent& event);
	void OnDomainVolumeFraction(wxCommandEvent& event);
	void OnQuickDomainVolumeFraction(wxCommandEvent& event);

	string GetTextileSelection();
	void UpdateRenderingPage();
	void UpdateModellingPage();
	void UpdateOutliner();
	COLOR GetUserSelectedColor();

	wxAuiManager m_Manager;
	wxAuiNotebook *m_pViewerNotebook;
	wxAuiNotebook *m_pLogNotebook;
    wxTextCtrl *m_pTexGenOutput;
	CControlsWindow *m_pControls;
	wxPanel* m_pOutliner;

	map<string, CTexGenRenderWindow*> m_RenderWindows;
	DECLARE_EVENT_TABLE()
};

class CNoteBookHandler : public wxEvtHandler
{
public:
	CNoteBookHandler(CTexGenMainFrame &MainFrame) : m_MainFrame(MainFrame) {}

protected:
    void OnSetFocus(wxFocusEvent &event);

	CTexGenMainFrame &m_MainFrame;

	DECLARE_EVENT_TABLE()
};

class COutlineHandler : public wxEvtHandler
{
public:
	COutlineHandler(CTexGenMainFrame &MainFrame) : m_MainFrame(MainFrame) {}

protected:
	void OnSelectionChanged(wxTreeEvent& event);
	void OnDeleteSelected(wxCommandEvent& event);
	void OnInsertNode(wxCommandEvent& event);
	void OnDuplicateYarn(wxCommandEvent& event);

	CTexGenMainFrame &m_MainFrame;

	DECLARE_EVENT_TABLE()
};

class CAbaqusVoxelInput : public wxDialog
{
public:
	CAbaqusVoxelInput( wxWindow* parent );

private:
	void OnOffsetUpdate(wxUpdateUIEvent& event);

	DECLARE_EVENT_TABLE()
};

class CVolumeMeshOptions : public wxDialog
{
public:
	CVolumeMeshOptions( wxWindow* parent );

private:
	void OnPeriodicBoundariesUpdate(wxUpdateUIEvent& event);

	DECLARE_EVENT_TABLE()
};
