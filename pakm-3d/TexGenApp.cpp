#include "TexGenApp.h"
#include "LoggerGUI.h"

IMPLEMENT_APP(CTexGenApp)

void TextileCallback(string TextileName, bool bAdded)
{
	if (wxTheApp)
	{
		CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
		if (pMainFrame)
		{
			pMainFrame->TextileCallback(TextileName, bAdded);
		}
	}
}

void RendererCallback()
{
	if (wxTheApp)
	{
		CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
		if (pMainFrame)
		{
			pMainFrame->RendererCallback();
		}
	}	
}

extern "C" void init_Embedded(void);

CTexGenApp::CTexGenApp()
: m_pMainFrame(NULL)
{
	Py_Initialize();

	CTexGen::Instance().SetMessages( true, CLoggerGUI() );
	
	CTexGen::Instance().SetTextileCallback(TextileCallback);

	// Register the swig embedded module with python
	init_Embedded();

	// Hook into the standard output of Python
//	Py_InitModule("emb", EmbMethods);
	PyRun_SimpleString("import _Embedded");
	PyRun_SimpleString("class OutputCatcher:\n"
						"    def __init__(self, type):\n"
						"        self.data = type\n"
						"    def write(self, text):\n"
						"        _Embedded.ReceivePythonOutput(text, self.data)\n");	// This code causes program to crash on Py_Finalize() (only in Python 2.3 it seems)
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.insert(0, '.\\Python\\libxtra')");
	PyRun_SimpleString("sys.path.insert(0, '.\\Python\\libstd')");
	// Redirect output to the OutputCatcher python class
	PyRun_SimpleString("sys.stdout = OutputCatcher('script_stdout')");
	PyRun_SimpleString("sys.stderr = OutputCatcher('script_stderr')");
//	wxString SetPath = "sys.path.append('" + wxGetCwd() + "')";
//	PyRun_SimpleString(SetPath.c_str());
}

CTexGenApp::~CTexGenApp()
{
	// Restore output to original state
	PyRun_SimpleString("sys.stdout = sys.__stdout__");
	PyRun_SimpleString("sys.stderr = sys.__stderr__");

	Py_Finalize();
}

bool CTexGenApp::OnInit()
{
	SetAppName(wxT("TexGen"));

	wxConfigBase::Set(wxConfigBase::Create());

	wxXmlResource::Get()->InitAllHandlers();
	wxXmlResource::Get()->Load(wxStandardPaths::Get().GetDataDir() + wxT("/TexGen.xrc"));

	m_pMainFrame = new CTexGenMainFrame(wxT("Saturn PAKM 3D"), wxDefaultPosition, wxSize(500,450));
	m_pMainFrame->Show(TRUE);
	m_pMainFrame->OnInit();

	SetTopWindow(m_pMainFrame);


	return wxApp::OnInit();
}

int CTexGenApp::OnExit()
{
	m_pMainFrame = NULL;

	return wxApp::OnExit();
}

void CTexGenApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddParam(wxT("Filename(s) of TexGen file(s) or Python script(s)"),
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL|wxCMD_LINE_PARAM_MULTIPLE);
	wxApp::OnInitCmdLine(parser);
}

bool CTexGenApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	wxArrayString filenames;
	for (size_t i = 0; i < parser.GetParamCount(); i++) {
		filenames.Add(parser.GetParam(i));
	}
	if (m_pMainFrame) {
		m_pMainFrame->ProcessFiles(filenames);
	}
	return wxApp::OnCmdLineParsed(parser);
}
