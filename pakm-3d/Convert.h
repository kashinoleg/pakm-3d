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