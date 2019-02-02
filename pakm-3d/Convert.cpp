#include "Convert.h"
using namespace TexGen;
using namespace std;

string ConvertUTF::utf8_encode(const wstring &wstr) {
	//TODO
	if (wstr.empty()) {
		return string();
	}
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

wstring ConvertUTF::utf8_decode(const string &str) {
	if (str.empty()) {
		return wstring();
	}
	int size_needed = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), NULL, 0);
	wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
