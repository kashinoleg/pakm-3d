#pragma once
#include <vector>
#include "wx/wx.h"
#include "wx/listbox.h"
#include "WindowIDs.h"
#include "TexGenApp.h"

using namespace std;

class CTextileLayersDialog : public wxDialog
{
protected:
	wxListBox* m_pTextilesListBox;
	wxListBox* m_pLayersListBox;
public:
	CTextileLayersDialog();
	~CTextileLayersDialog(void);

	void GetLayerNames( vector<string>& LayerNames );

protected:
	void BuildDialog();
	void OnOK(wxCommandEvent& event);

	void OnAddLayer(wxCommandEvent& event);
	void OnRemoveLayer(wxCommandEvent& event);
	void OnMoveLayerUp(wxCommandEvent& event);
	void OnMoveLayerDown(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};
