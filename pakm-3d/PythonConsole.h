#pragma once
#include "PythonWrapper.h"
#include "CompleteDialog.h"

/*class CPythonConsole : public wxTextCtrl
{
private:
	bool m_bUseCompleteWindow;
	long m_iInputPosition;
	long m_iCompletePosition;
	long m_iCompleteState;
	CPythonWrapper &m_PythonWrapper;
	CCompleteDialog m_CompleteDialog;
public:
    CPythonConsole(CPythonWrapper &PythonWrapper, wxWindow *parent,
		wxWindowID id, const wxString &value,
		const wxPoint &pos, const wxSize &size, int style = 0);

private:
	void ShowAutoCompleteWindow(const vector<string> &Completions);
	void HideAutoCompleteWindow();
	void UpdateAutoCompleteWindow();
	void OnChar(wxKeyEvent& event);
	void OnText(wxCommandEvent& event);
	void OnSetFocus(wxFocusEvent& event) {};
	void OnKillFocus(wxFocusEvent& event);

	DECLARE_EVENT_TABLE()
};
//*/