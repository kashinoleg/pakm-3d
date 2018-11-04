#pragma once
#include "Misc.h"
#include <string>
#include <sstream>
#include <iostream>

namespace TexGen {
	/// Macros used to report the file name and line number to the TexGenError and TexGenLog functions
	/**
	Also converts stream to string making it easier to insert values into the error message without need to
	use the stringify function. E.g. TEXGENERROR("Value is not high enough: " << iValue);
	*/

	#define TGERROR(MESSAGE) \
	{ \
		std::ostringstream o; \
		o << MESSAGE; \
		GetLogger().TexGenError(__FILE__, __LINE__, o.str()); \
	}

	#define TGLOG(MESSAGE) \
	{ \
		std::ostringstream o; \
		o << MESSAGE; \
		GetLogger().TexGenLog(__FILE__, __LINE__, o.str()); \
	}

	/// Macros to increase and decrease indentation in the logging
	/**
	Indentation can be done in two ways, by calling INCREASELOGINDENT followed by DECREASELOGINDENT. Care must
	be taken to make sure that each INCREASELOGINDENT is ALWAYS matched with a DECREASELOGINDENT (it can be easy
	to get a miss match when a "return" occurs in the middle of a function).

	A safer way to indent things is by calling AUTOLOGINDENT. This macro actually creates an instance of the CLogIndent
	class which has a constructor that calls INCREASELOGINDENT and destructor that calls DECREASELOGINDENT. Thus a call
	to increase indent is ALWAYS matched with a call to decrease indent. The decrease indent occurs when the variable
	exits the current scope, i.e. before exiting a function, an if statement, loop, etc... Note that this macro cannot
	be placed twice within the same control loop since the class instance will already be declared with the same name.
	*/
	#define TGLOGINCREASEINDENT() \
	{ \
		GetLogger().IncreaseIndent(); \
	}

	#define TGLOGDECREASEINDENT() \
	{ \
		GetLogger().DecreaseIndent(); \
	}

	#define TGLOGAUTOINDENT() CLogIndent AutoLogIndentVariable;

	/// Combines the TGLOG macro and TGLOGAUTOINDENT macro in one
	#define TGLOGINDENT(MESSAGE) TGLOG(MESSAGE) TGLOGAUTOINDENT()

	/// Class to handle log indenting
	/**
	This class consists only of a constructor and destructer where the constructor increases
	the indent in the log while the destructor decreases the indent in the log. Should be used
	in conjunction with the TGLOGAUTOINDENT() macro.
	*/
	class CLASS_DECLSPEC CLogIndent
	{
	public:
		CLogIndent();
		~CLogIndent();
	};

	/// Abstract base class to act as an interface between texgen and the logger
	class CLASS_DECLSPEC CLogger {
	public:
		CLogger(void);
		virtual ~CLogger(void);

		virtual CLogger *Copy() const = 0;
		virtual void TexGenError(std::string FileName, int iLineNumber, std::string Message) = 0;
		virtual void TexGenLog(std::string FileName, int iLineNumber, std::string Message) = 0;
		void IncreaseIndent() { ++m_iIndent; }
		void DecreaseIndent() { if (m_iIndent>0) --m_iIndent; }

	protected:
		int m_iIndent;
	};

	/// Logger used to print all log and error messages to the screen
	class CLASS_DECLSPEC CLoggerScreen : public CLogger
	{
	public:
		CLogger *Copy() const { return new CLoggerScreen(*this); }
		void TexGenError(std::string FileName, int iLineNumber, std::string Message);
		void TexGenLog(std::string FileName, int iLineNumber, std::string Message);

	protected:
	};

	/// Logger used to send all log and error messages into a black hole
	class CLASS_DECLSPEC CLoggerNull : public CLogger
	{
	public:
		CLogger *Copy() const { return new CLoggerNull(*this); }
		void TexGenError(std::string FileName, int iLineNumber, std::string Message) {;}
		void TexGenLog(std::string FileName, int iLineNumber, std::string Message) {;}
		void TexGenLog(std::string message)
		{
			std::ostringstream o;
			o << message;
			TexGenLog(__FILE__, __LINE__, o.str());
		}
	protected:
	};

	CLASS_DECLSPEC CLogger &GetLogger();

}
