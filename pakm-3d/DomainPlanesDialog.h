#pragma once
#include "wx/wx.h"
#include <wx/xrc/xmlres.h>
#include <wx/listctrl.h>
#include "wx\button.h"
#include "wx\stattext.h"
#include "wx\valtext.h"
#include "TexGen.h"
#include "WindowIDs.h"
using namespace TexGen;
using namespace std;

class CDomainPlanesDialog : public wxDialog {
protected:
	wxListView* m_pListView;
	wxString m_X;
	wxString m_Y;
	wxString m_Z;
	wxString m_D;
public:
	CDomainPlanesDialog(wxWindow* parent, wxWindowID id = wxID_ANY);
	~CDomainPlanesDialog(void) {};

	string GetCreateDomainCommand();
	CDomainPlanes CreateDomain();
	void LoadSettings(const CDomainPlanes &DomainPlanes);
	bool CreateBox();

protected:
	void BuildDialog();
	void OnButton(wxCommandEvent& event);
	void OnItemSelected(wxListEvent& event);

	DECLARE_EVENT_TABLE()
};
