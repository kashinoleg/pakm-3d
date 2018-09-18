#pragma once
#include <wx/dnd.h>
#include "TexGenApp.h"
#include "TexGenMainFrame.h"

class CFileDropTarget : public wxFileDropTarget {
public:
	CFileDropTarget(void) {};
	~CFileDropTarget(void) {};

	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
};
