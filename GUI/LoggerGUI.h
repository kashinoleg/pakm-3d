#pragma once
#include "TexGenMainFrame.h"
#include "TexGenApp.h"

/// Logger used to print all log and error messages to the screen
class CLoggerGUI : public CLogger
{
protected:
	bool m_bInteractive;
public:
	CLoggerGUI(bool bInteractive = false) {
		m_bInteractive = bInteractive;
	}

	CLogger *Copy() const {
		return new CLoggerGUI(*this);
	}

	void TexGenError(std::string FileName, int iLineNumber, std::string Message)
	{
		if (wxTheApp)
		{
			CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
			if (pMainFrame)
			{
				string ProcessedMessage;
				for (int i = 0; i < m_iIndent; i++) {
					ProcessedMessage += "  ";
				}
				ProcessedMessage += Message + "\n";
				pMainFrame->ReceiveOutput(ProcessedMessage, CTexGenMainFrame::OUTPUT_TEXGEN, true, m_bInteractive);
			}
		}
	}

	void TexGenLog(std::string FileName, int iLineNumber, std::string Message)
	{
		if (wxTheApp)
		{
			CTexGenMainFrame *pMainFrame = ((CTexGenApp*)wxTheApp)->GetMainFrame();
			if (pMainFrame)
			{
				string ProcessedMessage;
				for (int i = 0; i < m_iIndent; i++) {
					ProcessedMessage += "  ";
				}
				ProcessedMessage += Message + "\n";
				pMainFrame->ReceiveOutput(ProcessedMessage, CTexGenMainFrame::OUTPUT_TEXGEN, false, m_bInteractive);
			}
		}
	}
};
