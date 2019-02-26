#include "ConvertString.h"
using namespace TexGen;
using namespace std;

string Convert::ToString(wxString String)
{
	return string(String.mb_str(wxConvUTF8));
}

wxString Convert::ToWxString(string String)
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

const string Convert::NAME_POISSONS_RATIO_X = "Poisson's Ratio X";
const string Convert::NAME_POISSONS_RATIO_Y = "Poisson's Ratio Y";
const string Convert::NAME_POISSONS_RATIO_Z = "Poisson's Ratio Z";
const string Convert::NAME_YARN_LINEAR_DENSITY = "Yarn Linear Density";
const string Convert::NAME_FIBRE_DENSITY = "Fibre Density";
const string Convert::NAME_FIBRE_AREA = "Total Fibre Area";
const string Convert::NAME_FIBRE_DIAMETER = "Fibre Diameter";
const string Convert::NAME_YOUNGS_MODULUS_X = "Young's Modulus X";
const string Convert::NAME_YOUNGS_MODULUS_Y = "Young's Modulus Y";
const string Convert::NAME_YOUNGS_MODULUS_Z = "Young's Modulus Z";
const string Convert::NAME_SHEAR_MODULUS_XY = "Shear Modulus XY";
const string Convert::NAME_SHEAR_MODULUS_XZ = "Shear Modulus XZ";
const string Convert::NAME_SHEAR_MODULUS_YZ = "Shear Modulus YZ";
const string Convert::NAME_ALPHA_X = "Alpha X";
const string Convert::NAME_ALPHA_Y = "Alpha Y";
const string Convert::NAME_ALPHA_Z = "Alpha Z";
const string Convert::NAME_AREAL_DENSITY = "Areal Density";
const string Convert::NAME_FIBRES_PER_YARN = "Fibres Per Yarn";

CProperties_ID Convert::ToProperty(wxString name)
{
	return ToProperty(ToString(name));
}

CProperties_ID Convert::ToProperty(string name)
{
	if (name == NAME_POISSONS_RATIO_X)
	{
		return ID_POISSONS_RATIO_X;
	}
	if (name == NAME_POISSONS_RATIO_Y)
	{
		return ID_POISSONS_RATIO_Y;
	}
	if (name == NAME_POISSONS_RATIO_Z)
	{
		return ID_POISSONS_RATIO_Z;
	}
	if (name == NAME_YARN_LINEAR_DENSITY)
	{
		return ID_YARN_LINEAR_DENSITY;
	}
	if (name == NAME_FIBRE_DENSITY)
	{
		return ID_FIBRE_DENSITY;
	}
	if (name == NAME_FIBRE_AREA)
	{
		return ID_FIBRE_AREA;
	}
	if (name == NAME_FIBRE_DIAMETER)
	{
		return ID_FIBRE_DIAMETER;
	}
	if (name == NAME_YOUNGS_MODULUS_X)
	{
		return ID_YOUNGS_MODULUS_X;
	}
	if (name == NAME_YOUNGS_MODULUS_Y)
	{
		return ID_YOUNGS_MODULUS_Y;
	}
	if (name == NAME_YOUNGS_MODULUS_Z)
	{
		return ID_YOUNGS_MODULUS_Z;
	}
	if (name == NAME_SHEAR_MODULUS_XY)
	{
		return ID_SHEAR_MODULUS_XY;
	}
	if (name == NAME_SHEAR_MODULUS_XZ)
	{
		return ID_SHEAR_MODULUS_XZ;
	}
	if (name == NAME_SHEAR_MODULUS_YZ)
	{
		return ID_SHEAR_MODULUS_YZ;
	}
	if (name == NAME_ALPHA_X)
	{
		return ID_ALPHA_X;
	}
	if (name == NAME_ALPHA_Y)
	{
		return ID_ALPHA_Y;
	}
	if (name == NAME_ALPHA_Z)
	{
		return ID_ALPHA_Z;
	}
	if (name == NAME_AREAL_DENSITY)
	{
		return ID_AREAL_DENSITY;
	}
	if (name == NAME_FIBRES_PER_YARN)
	{
		return ID_FIBRES_PER_YARN;
	}
}

wxString Convert::ToWxString(CProperties_ID Id)
{
	return ToWxString(ToString(Id));
}

string Convert::ToString(CProperties_ID Id)
{
	switch (Id)
	{
	case TexGen::ID_POISSONS_RATIO_X:
		return NAME_POISSONS_RATIO_X;
	case TexGen::ID_POISSONS_RATIO_Y:
		return NAME_POISSONS_RATIO_Y;
	case TexGen::ID_POISSONS_RATIO_Z:
		return NAME_POISSONS_RATIO_Z;
	case TexGen::ID_YARN_LINEAR_DENSITY:
		return NAME_YARN_LINEAR_DENSITY;
	case TexGen::ID_FIBRE_DENSITY:
		return NAME_FIBRE_DENSITY;
	case TexGen::ID_FIBRE_AREA:
		return NAME_FIBRE_AREA;
	case TexGen::ID_FIBRE_DIAMETER:
		return NAME_FIBRE_DIAMETER;
	case TexGen::ID_YOUNGS_MODULUS_X:
		return NAME_YOUNGS_MODULUS_X;
	case TexGen::ID_YOUNGS_MODULUS_Y:
		return NAME_YOUNGS_MODULUS_Y;
	case TexGen::ID_YOUNGS_MODULUS_Z:
		return NAME_YOUNGS_MODULUS_Z;
	case TexGen::ID_SHEAR_MODULUS_XY:
		return NAME_SHEAR_MODULUS_XY;
	case TexGen::ID_SHEAR_MODULUS_XZ:
		return NAME_SHEAR_MODULUS_XZ;
	case TexGen::ID_SHEAR_MODULUS_YZ:
		return NAME_SHEAR_MODULUS_YZ;
	case TexGen::ID_ALPHA_X:
		return NAME_ALPHA_X;
	case TexGen::ID_ALPHA_Y:
		return NAME_ALPHA_Y;
	case TexGen::ID_ALPHA_Z:
		return NAME_ALPHA_Z;
	case TexGen::ID_AREAL_DENSITY:
		return NAME_AREAL_DENSITY;
	case TexGen::ID_FIBRES_PER_YARN:
		return NAME_FIBRES_PER_YARN;
	default:
		break;
	}
}
