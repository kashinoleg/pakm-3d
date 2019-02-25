#pragma once
//#define wxUSE_UNICODE 1
#include "wx/wx.h"
#include "TexGen.h"
using namespace TexGen;

inline string ConvertString(wxString String)
{
	//return string(String);
	return std::string(String.mb_str(wxConvUTF8));
}

inline wxString ConvertString(string String)
{
	//return wxString(String);
	return wxString(String.c_str(), wxConvUTF8);
}

inline wxColour ConvertColor(COLOR Color)
{
	return wxColour((unsigned char)(Color.r * 255), (unsigned char)(Color.g * 255), (unsigned char)(Color.b * 255));
}

namespace TexGen
{
	using namespace std;

	class Convert
	{
	public:
		static string ToString(wxString String);

		static wxString ToString(string String);

		static int ToInt(wxString String);
		static int ToInt(string String);

		static double ToDouble(wxString String);
		static double ToDouble(string String);


	};
}