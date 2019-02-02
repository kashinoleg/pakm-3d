#pragma once
#include "wx/wx.h"
#include <vector>
#include <wx/grid.h>
#include "latin_random.hpp"
#include "TexGen.h"
#include "ConvertString.h"
#include "WindowIDs.h"
#include "TexGenApp.h"
#include "latin_random.hpp"
using namespace TexGen;
using namespace std;

enum
{
	CONSTANT,
	RANDOM,
	EDIT
};

class CLayersOffsetDialog : public wxDialog {
protected:
	wxGrid* m_pGrid;
	wxRadioBox* m_OptionsRadio;
	wxTextCtrl* m_XOffset;
	wxTextCtrl* m_YOffset;

	XY m_DomainSize;
public:
	CLayersOffsetDialog();
	~CLayersOffsetDialog(void) {};

	void LoadSettings( const vector<XY>& Offsets, XY& DomainSize );
	int GetOption();
	void GetEditOffsets( vector<XY>& Offsets );
	void GetConstantOffset( XY& Offset );

protected:
	void BuildDialog();
	void OnOK(wxCommandEvent& event);
	//void OnInit(wxInitDialogEvent& event);
	void OnSelectLayerOption(wxCommandEvent& event);
	void GetRandomOffsets();

	DECLARE_EVENT_TABLE()
};
