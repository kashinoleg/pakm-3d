# include "TestUtilities.h"

bool CompareFiles(string FileName1, string FileName2)
{
	ifstream Input1(FileName1.c_str());
	stringstream StringStream1;
	StringStream1 << Input1.rdbuf();
	ifstream Input2(FileName2.c_str());
	stringstream StringStream2;
	StringStream2 << Input2.rdbuf();
	return (StringStream1.str() == StringStream2.str());
}