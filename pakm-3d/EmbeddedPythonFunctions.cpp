#include "EmbeddedPythonFunctions.h"

CTexGenRenderer *CreateRenderWindow(string WindowName)
{
	if (wxTheApp)
	{
		CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
		if (pMainFrame)
		{
			CTexGenRenderWindow* pRenderWindow = pMainFrame->CreateRenderWindow(WindowName);
			if (pRenderWindow)
				return pRenderWindow->GetRenderer();
		}
	}
	return NULL;
}

bool DeleteRenderWindow(string WindowName)
{
	if (wxTheApp)
	{
		CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
		if (pMainFrame)
		{
			return pMainFrame->DeleteRenderWindow(WindowName);
		}
	}
	return false;
}

bool Refresh(string WindowName)
{
	if (wxTheApp)
	{
		CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
		if (pMainFrame)
		{
			return pMainFrame->RefreshTextile(WindowName);
		}
	}
	return false;
}
