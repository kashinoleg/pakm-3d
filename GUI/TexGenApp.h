#pragma once
#include <wx/stdpaths.h>
#include <wx/cmdline.h>
#include <wx/config.h>
#include "TexGenMainFrame.h"


class CTexGenApp: public wxApp {
protected:
	CTexGenMainFrame *m_pMainFrame;
protected:
	bool OnInit();
	int OnExit();
	bool OnCmdLineParsed(wxCmdLineParser& parser);
	void OnInitCmdLine(wxCmdLineParser& parser);
public:
	CTexGenApp();
	~CTexGenApp();

	CTexGenMainFrame* GetMainFrame() { return m_pMainFrame; }
};
