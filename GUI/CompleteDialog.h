#pragma once
#include "wx/wx.h"
#include <vector>
#include "Convert.h"
using namespace std;

class CCompleteDialog : public wxDialog {
protected:
	int m_iThumbSize;
	wxListBox *m_pListBox;
	bool m_bVisible;
public:
	CCompleteDialog(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);

	void SetCompleteOptions(const vector<string> &Strings);
	void MoveDown();
	void MoveUp();
	void MovePageDown();
	void MovePageUp();
	wxString GetSelectedText();
	void FakeHide();
	bool Visible() { return m_bVisible; }
};
