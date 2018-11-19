#include "CompleteDialog.h"

CCompleteDialog::CCompleteDialog(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
: wxDialog(parent, id, wxT(""), pos, size, wxSTAY_ON_TOP | wxNO_BORDER)
{
	m_pListBox = NULL;
	m_iThumbSize = 4;
	m_bVisible = false;
	m_pListBox = new wxListBox(this, wxID_ANY);
	wxFont ConsoleFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	m_pListBox->SetFont(ConsoleFont);
}

void CCompleteDialog::SetCompleteOptions(const vector<string> &Strings)
{
	wxArrayString ArrayString;
	for (size_t i = 0; i < Strings.size(); i++) {
		ArrayString.Add(ConvertString(Strings[i]));
	}
	m_pListBox->Set(ArrayString);
	if (m_pListBox->GetCount() > 0) {
		m_pListBox->SetSelection(0);
	}
	m_bVisible = true;
}

void CCompleteDialog::MoveDown()
{
	int iSelection = m_pListBox->GetSelection();
	int iCount = m_pListBox->GetCount();
	if (iSelection != wxNOT_FOUND && iSelection < iCount - 1) {
		m_pListBox->SetSelection(iSelection + 1);
	}
}

void CCompleteDialog::MoveUp()
{
	int iSelection = m_pListBox->GetSelection();
	if (iSelection != wxNOT_FOUND && iSelection > 0) {
		m_pListBox->SetSelection(iSelection - 1);
	}
}

void CCompleteDialog::MovePageDown()
{
	for (int i = 0; i < m_iThumbSize; i++) {
		MoveDown();
	}
}

void CCompleteDialog::MovePageUp()
{
	for (int i = 0; i < m_iThumbSize; i++) {
		MoveUp();
	}
}

wxString CCompleteDialog::GetSelectedText()
{
	return m_pListBox->GetStringSelection();
}

void CCompleteDialog::FakeHide()
{
	m_bVisible = false;
	SetSize(wxRect(0, 0, 0, 0));
}
