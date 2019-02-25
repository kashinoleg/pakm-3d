#include "ConvertString.h"
using namespace TexGen;
using namespace std;

string Convert::ToString(wxString String)
{
	return string(String.mb_str(wxConvUTF8));
}

wxString Convert::ToString(string String)
{
	return wxString(String.c_str(), wxConvUTF8);
}

int Convert::ToInt(string String)
{
	return stoi(String);
}

int Convert::ToInt(wxString String)
{
	return ToInt(ToString(String));
}

double Convert::ToDouble(string String)
{
	return stod(String);
}

double Convert::ToDouble(wxString String)
{
	return ToDouble(ToString(String));
}
