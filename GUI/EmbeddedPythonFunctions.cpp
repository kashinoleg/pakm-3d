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

CTexGenRenderer *GetRenderWindow(string WindowName)
{
	if (wxTheApp)
	{
		CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
		if (pMainFrame)
		{
			CTexGenRenderWindow* pRenderWindow = pMainFrame->GetRenderWindow(WindowName);
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

void ReceivePythonOutput(string Output, string Type)
{
	if (wxTheApp)
	{
		CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
		if (pMainFrame)
		{
			if (Type == "script_stdout")
				pMainFrame->ReceiveOutput(Output, CTexGenMainFrame::OUTPUT_PYTHON, false, false);
			else if (Type == "script_stderr")
				pMainFrame->ReceiveOutput(Output, CTexGenMainFrame::OUTPUT_PYTHON, true, false);
			else if (Type == "interactive_stdout")
				pMainFrame->ReceiveOutput(Output, CTexGenMainFrame::OUTPUT_PYTHON, false, true);
			else if (Type == "interactive_stderr")
				pMainFrame->ReceiveOutput(Output, CTexGenMainFrame::OUTPUT_PYTHON, true, true);
		}
	}
}

#include "Embedded_wrap.cxx"
