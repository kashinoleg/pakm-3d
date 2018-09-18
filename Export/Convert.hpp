#include <string>
#include <Windows.h>

using namespace std;

namespace TexGen {
	class ConvertUTF {
	public:
		// Convert a wide Unicode string to an UTF8 string
		string utf8_encode(const wstring &wstr);
		// Convert an UTF8 string to a wide Unicode String
		wstring utf8_decode(const string &str);
	};
}
