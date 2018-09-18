#pragma once
#include "wxVTKRenderWindowInteractor.h"
#include "TexGenRenderer.h"
#include "Modeller.h"

class CTexGenRenderWindow : public wxVTKRenderWindowInteractor
{
protected:
	CTexGenRenderer *m_pRenderer;
	CModeller *m_pModeller;
public:
	CTexGenRenderWindow(void);
	CTexGenRenderWindow(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
	~CTexGenRenderWindow(void);

	void CleanUp();
	void AttachModeller(string TextileName);
    void OnButtonDown(wxMouseEvent &event);
    void OnButtonUp(wxMouseEvent &event);
    void OnKeyDown(wxKeyEvent &event);
	void OnKeyUp(wxKeyEvent &event);

	CTexGenRenderer *GetRenderer() { return m_pRenderer; }
	CModeller *GetModeller() { return m_pModeller; }
protected:
	void Initialise();

	DECLARE_EVENT_TABLE()
};
