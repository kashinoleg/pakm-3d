#pragma once
//#define wxUSE_UNICODE 1
#include "wx/wx.h"
#include "TexGen.h"
#include "Properties.h"
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
	private:
		static const string NAME_POISSONS_RATIO_X;
		static const string NAME_POISSONS_RATIO_Y;
		static const string NAME_POISSONS_RATIO_Z;
		static const string NAME_YARN_LINEAR_DENSITY;
		static const string NAME_FIBRE_DENSITY;
		static const string NAME_FIBRE_AREA;
		static const string NAME_FIBRE_DIAMETER;
		static const string NAME_YOUNGS_MODULUS_X;
		static const string NAME_YOUNGS_MODULUS_Y;
		static const string NAME_YOUNGS_MODULUS_Z;
		static const string NAME_SHEAR_MODULUS_XY;
		static const string NAME_SHEAR_MODULUS_XZ;
		static const string NAME_SHEAR_MODULUS_YZ;
		static const string NAME_ALPHA_X;
		static const string NAME_ALPHA_Y;
		static const string NAME_ALPHA_Z;
		static const string NAME_AREAL_DENSITY;
		static const string NAME_FIBRES_PER_YARN;
	public:
		static string ToString(wxString String);
		static string ToString(CProperties_ID Id);

		static int ToInt(wxString String);
		static int ToInt(string String);

		static double ToDouble(wxString String);
		static double ToDouble(string String);

		static CProperties_ID ToProperty(string name);
		static CProperties_ID ToProperty(wxString name);

		static wxString ToWxString(CProperties_ID Id);
		static wxString ToWxString(string String);
	};
}