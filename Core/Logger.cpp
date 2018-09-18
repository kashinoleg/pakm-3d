#include "Logger.h"
#include "TexGen.h"

using namespace TexGen;

CLogIndent::CLogIndent()
{
	TGLOGINCREASEINDENT();
}

CLogIndent::~CLogIndent()
{
	TGLOGDECREASEINDENT();
}

CLogger::CLogger(void)
: m_iIndent(0)
{
}

CLogger::~CLogger(void)
{
}

/// Function to report a modelling error in texgen, to be used in conjunction with TGERROR() macro
/// so that the filename and linenumbers are filled in automatically
void CLoggerScreen::TexGenError(std::string FileName, int iLineNumber, std::string Message)
{
	std::cerr << FileName << "(" << iLineNumber << ") : " << Message << std::endl;
}

/// Function to report log messages from texgen, to be used in conjunction with TGLOG() macro
/// so that the filename and linenumbers are filled in automatically
void CLoggerScreen::TexGenLog(std::string FileName, int iLineNumber, std::string Message)
{
	int i;
	for (i=0; i<m_iIndent; ++i)
		std::cout << "  ";
	std::cout << Message << std::endl;
}

namespace TexGen
{
	CLogger &GetLogger()
	{
		return TEXGEN.GetLogger();
	}
}









