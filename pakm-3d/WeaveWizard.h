#pragma once
#include "wx/wx.h"
#include <wx/valgen.h>
#include <wx/spinctrl.h>
#include <wx/wizard.h>

#include "TexGen.h"
using namespace TexGen;
using namespace std;

class wxWeavePatternCtrl;

class CWeaveWizard : public wxWizard
{
public:
	CWeaveWizard(wxWindow* parent, wxWindowID id = wxID_ANY);
	~CWeaveWizard(void);

	bool RunIt();
	string GetCreateTextileCommand(string ExistingTextile = "");
	void LoadSettings(const CTextileWeave2D &Weave);
	void LoadSettings(const CShearedTextileWeave2D &Weave);

protected:
	void OnWizardPageChanging(wxWizardEvent& event);
	void BuildPages();
	wxWizardPageSimple* BuildFirstPage();
	wxDialog* BuildWeavePatternDialog();
	
	bool RebuildWeavePatternCtrl();
	bool GetPatternCell(int i, int j);
//	void SetPatternCell(int i, int j, bool bVal);
	bool HasNextPage(wxWizardPage *page);
	void OnWidthChanged(wxCommandEvent& event) { m_bWidthChanged = true; }
	void OnSpacingChanged(wxCommandEvent& event) { m_bSpacingChanged = true; }
	void OnThicknessChanged(wxCommandEvent& event) { m_bThicknessChanged = true; }
	void OnInit(wxInitDialogEvent& event) {m_bWidthChanged = m_bSpacingChanged = m_bThicknessChanged = false;}

	void OnRefine(wxCommandEvent& event);
	void On3DWeave(wxCommandEvent& event);
	void OnInPlaneTangents(wxCommandEvent& event);
	void OnWeavePattern(wxCommandEvent& event);
	void RefreshGapTextBox();
	void OnShear(wxCommandEvent& event);
	void OnDomain(wxCommandEvent& event);
	void OnShearedDomain(wxCommandEvent& event);
	void OnLayers(wxCommandEvent& event);

	wxSpinCtrl *m_pXYarnsSpin;
	wxSpinCtrl *m_pYYarnsSpin;
	wxWeavePatternCtrl *m_pWeavePatternCtrl;
	wxString m_YarnSpacing;
	wxString m_YarnWidth;
	wxString m_FabricThickness;
	wxString m_GapSize;
	wxString m_ShearAngle;
	wxSpinCtrl *m_pLayersSpin;

	bool m_bCreateDomain;
	bool m_bRefine;
	bool m_bInPlaneTangents;
	bool m_b3DWeave;
	bool m_bShear;
	bool m_bShearedDomain;
	bool m_bLayers;
	bool m_bAddedDomainHeight;

	bool m_bWidthChanged;
	bool m_bSpacingChanged;
	bool m_bThicknessChanged;

	wxWizardPageSimple *m_pFirstPage;
	wxDialog *m_pWeavePatternDialog;

	DECLARE_EVENT_TABLE()
};
