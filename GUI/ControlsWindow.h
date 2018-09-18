#pragma once
#include "wx/wx.h"
#include <wx/choicebk.h>
#include "TexGenRenderer.h"
#include "TexGen.h"
#include "WindowIDs.h"
#include "Modeller.h"
#include "CustomWidget.h"
using namespace TexGen;

class CModeller;

class CControlsWindow : public wxChoicebook
{
public:
	CControlsWindow(wxMenuBar* pMenuBar, wxWindow* parent, wxWindowID id);
	~CControlsWindow(void) {};

	void UpdateModellingPage(const CModeller *pModeller);
	void UpdateRenderingPage(const CTexGenRenderer *pRenderer);
	void UpdatePythonPage(bool bRecording);
	void UpdatePositionText(const CModeller *pModeller);
	void UpdateSnapSize(const CModeller *pModeller);

	bool m_bUpdatingPositionText;

protected:
	void BuildControls();
	void BuildTextilesPage();
	void BuildModellerPage();
	void BuildRenderingPage();
	void BuildDomainsPage();
	void BuildPythonPage();
	void ResizePages();
	void BuildOptionsPage();

	void UpdateRenderWidget(int iID, const CTexGenRenderer *pRenderer, CTexGenRenderer::PROP_TYPE Type);
	void UpdateCheckWidget(int iID, bool bChecked);

	wxMenuBar* m_pMenuBar;

	wxString m_PositionX;
	wxString m_PositionY;
	wxString m_PositionZ;
	wxString m_SnapSize;
};
