#pragma once
#include "TexGenMainFrame.h"
#include "TexGenApp.h"

/// Logger used to print all log and error messages to the screen
class CLoggerGUI : public CLogger
{
public:
	CLoggerGUI() {}

	CLogger *Copy() const
	{
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
				pMainFrame->ReceiveOutput(ProcessedMessage, true);
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
				for (int i = 0; i < m_iIndent; i++)
				{
					ProcessedMessage += "  ";
				}
				ProcessedMessage += Message + "\n";
				pMainFrame->ReceiveOutput(ProcessedMessage, false);
			}
		}
	}
};
