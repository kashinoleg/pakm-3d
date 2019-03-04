#pragma once
#include "SectionSelect.h"
#include "WindowIDs.h"
#include "TexGenApp.h"

class CInterpPositionItem : public wxClientData
{
public:
	CInterpPositionItem(const CSection &Section = CSectionEllipse(1, 1)) : m_dPosition(0), m_Section(Section) {}
	~CInterpPositionItem(void) {;}

	double m_dPosition;
	CObjectContainer<CSection> m_Section;
};

class CInterpNodesItem : public wxClientData
{
public:
	CInterpNodesItem(int iNode, const CSection &Section = CSectionEllipse(1, 1)) : m_iNode(iNode), m_Section(Section) {}
	~CInterpNodesItem(void) {;}

	int m_iNode;
	CObjectContainer<CSection> m_Section;
};

class CYarnSectionSelect : public wxDialog
{
public:
	CYarnSectionSelect();
	~CYarnSectionSelect(void);

	CYarnSection* GetCreateSection();
	void SetNumberOfNodes(int iNumNodes);
	void LoadSettings(const CYarnSection &YarnSection);

protected:
	void BuildDialog();
	void OnOK(wxCommandEvent& event);
	void OnConstantSection(wxCommandEvent& event);
	void OnInit(wxInitDialogEvent& event);
	void SetDefaultSection(const CSection &Section);

	void OnInterpPositionInsert(wxCommandEvent& event);
	void OnInterpPositionRemove(wxCommandEvent& event);
	void OnInterpPositionSection(wxCommandEvent& event);
	void OnInterpPositionSlider(wxScrollEvent &event);
	void OnInterpPositionText(wxCommandEvent &event);
	void OnInterpPositionSelectionChanged(wxCommandEvent &event);

	void UpdateInterpPositionListboxText();
	void UpdateInterpPositionText();
	void UpdateInterpPositionSlider();
	void UpdateInterpPositionItemText(int iSelection);


	void OnInterpNodesInsert(wxCommandEvent& event);
	void OnInterpNodesRemove(wxCommandEvent& event);
	void OnInterpNodesSection(wxCommandEvent& event);

	void UpdateInterpNodesListboxText();
	void UpdateInterpNodesItemText(int iSelection);	

	// Returns true if some, but not all, of sections are rectangle sections
	bool CheckRectangleSections( wxListBox* pListBox, string str );
	// Check if any of sections within hybrid are rectangle sections
	bool CheckHybridSection( const CSection& Hybrid );

	wxString m_Position;
	int m_iSliderPosition;

	bool m_bPolar, m_bRamped, m_bConstMesh;

	CObjectContainer<CSection> m_ConstactSection;

	DECLARE_EVENT_TABLE()
};
