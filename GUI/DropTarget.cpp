#include "DropTarget.h"

bool CFileDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) {
	if (wxTheApp) {
		CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
		if (pMainFrame) {
			pMainFrame->ProcessFiles(filenames);
		}
	}
	return true;
}
