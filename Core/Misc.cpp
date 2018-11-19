#include "stdafx.h"
#include "Misc.h"
#include "units.h"
#include "Logger.h"

using namespace std;

namespace TexGen {
	COLOR GetIndexedColor(int iIndex) {
		double ColorArray[][3] = {
			{  1,   0,   0},
			{  0,   1,   0},
			{  0,   0,   1},
			{  1,   1,   0},
			{  0,   1,   1},
			{  1,   0,   1},
			{  1,   0, 0.5},
			{  0, 0.5,   1},
			{  1, 0.5,   0},
			{0.5,   1,   0},
		};
		double* Color = ColorArray[iIndex % (sizeof(ColorArray) / (3 * sizeof(double)))];
		return COLOR(Color[0], Color[1], Color[2]);
	}

	void CopyToRange(vector<XYZ> &Offsets, XYZ Vector, int iLowerLimit, int iUpperLimit) {
		vector<XYZ> OriginalOffsets = Offsets;
		Offsets.clear();
		for (int i = iLowerLimit; i <= iUpperLimit; i++) {
			for (XYZ offset : OriginalOffsets) {
				Offsets.push_back(offset + i * Vector);
			}
		}
	}

	// Warning possible memory leaks contained within the units library!!!
	double ConvertUnits(double dValue, string SourceUnits, string TargetUnits) {
		UnitConvert convert;
		double dResult = dValue * convert.convert(SourceUnits, TargetUnits);
		if (convert.isError()) {
			TGERROR("Error converting units: " << convert.getErrorMsg());
		}
		return dResult;
	}

	bool CompatibleUnits(string SourceUnits, string TargetUnits, string *pErrorMessage) {
		UnitConvert convert;
		convert.convert(SourceUnits, TargetUnits);
		if (convert.isError()) {
			if (pErrorMessage) {
				*pErrorMessage = convert.getErrorMsg();
			}
			return false;
		}
		else {
			return true;
		}
	}

	string ReduceUnits(string Units) {
		UnitConvert convert;
		string Result = convert.units_reduce(Units);
		if (convert.isError()) {
			TGERROR("Error reducing units: " << convert.getErrorMsg());
		}
		return Result;
	}

	void AddNewUnits(string NewUnit, string BaseUnits) {
		UnitConvert convert;
		convert.add(NewUnit, BaseUnits);
		if (convert.isError()) {
			TGERROR("Error adding new units: " << convert.getErrorMsg());
		}
	}

	string StripPath(string Filename) {
		int iPos = Filename.find_last_of("\\/");
		if (iPos == string::npos) {
			return Filename;
		}
		else {
			return Filename.substr(iPos + 1);
		}
	}

	void AddExtensionIfMissing(string &Filename, string Extension) {
		if (Filename.size() < Extension.size()
			|| Filename.rfind(Extension) != Filename.size() - Extension.size()) {
			Filename += Extension;
		}
	}

	string RemoveExtension(string &Filename, string Extension) {
		int iPos = Filename.rfind(Extension);
		if (iPos == string::npos) {
			return Filename;
		}
		else {
			return Filename.substr(0, iPos);
		}
	}

	string ReplaceFilenameSpaces(string Filename) {
		int iPos = Filename.find_last_of("\\/");
		int iEnd = Filename.size();
		string str;
		if (iPos == string::npos) {
			str = Filename;
		}
		else {
			str = Filename.substr(iPos + 1);
		}	

		size_t found = str.find_first_of(" ");
		while (found != string::npos) {
			str[found] = '_';
			found = str.find_first_of(" ", found + 1);
		}

		if (iPos == string::npos) {
			return str;
		}
		else {
			return Filename.replace(iPos + 1, iEnd, str);
		}
	}
}
