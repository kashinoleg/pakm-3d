#pragma once
#include "TiXmlNode.hpp"

/** In correct XML the declaration is the first entry in the file.
@verbatim
<?xml version="1.0" standalone="yes"?>
@endverbatim

TinyXml will happily read or write files without a declaration,
however. There are 3 possible attributes to the declaration:
version, encoding, and standalone.

Note: In this version of the code, the attributes are
handled as special cases, not generic attributes, simply
because there can only be at most 3 and they are always the same.
*/

using namespace std;
class TiXmlDeclaration : public TiXmlNode {
private:
	string version;
	string encoding;
	string standalone;
protected:
	void CopyTo(TiXmlDeclaration* target) const {
		TiXmlNode::CopyTo(target);

		target->version = version;
		target->encoding = encoding;
		target->standalone = standalone;
	}
	// used to be public
	virtual void StreamIn(std::istream * in, std::string * tag);
public:
	/// <summary>
	/// Construct an empty declaration.
	/// </summary>
	TiXmlDeclaration() : TiXmlNode(TiXmlNodeType::TINYXML_DECLARATION) {}

	/// <summary>
	/// Constructor.
	/// </summary>
	TiXmlDeclaration(const string& _version, const string& _encoding, const string& _standalone) : TiXmlNode(TiXmlNodeType::TINYXML_DECLARATION)
	{
		version = _version;
		encoding = _encoding;
		standalone = _standalone;
	}

	TiXmlDeclaration(const TiXmlDeclaration& copy) : TiXmlNode(TiXmlNodeType::TINYXML_DECLARATION) {
		copy.CopyTo(this);
	}

	TiXmlDeclaration& operator=(const TiXmlDeclaration& copy) {
		Clear();
		copy.CopyTo(this);
		return *this;
	}

	virtual ~TiXmlDeclaration() {}

	/// <summary>
	/// Version. Will return an empty string if none was found.
	/// </summary>
	const string& Version() const {
		return version;
	}

	/// <summary>
	/// Encoding. Will return an empty string if none was found.
	/// </summary>
	const string& Encoding() const {
		return encoding;
	}

	/// <summary>
	/// Is this a standalone document?
	/// </summary>
	const string& Standalone() const {
		return standalone;
	}

	/// <summary>
	/// Creates a copy of this Declaration and returns it.
	/// </summary>
	virtual TiXmlNode* Clone() const {
		TiXmlDeclaration* clone = new TiXmlDeclaration();
		if (!clone) {
			return 0;
		}
		else {
			CopyTo(clone);
			return clone;
		}
	}


	/// <summary>
	/// Print this declaration to a FILE stream.
	/// </summary>
	virtual void Print(FILE* cfile, int /*depth*/, std::string* str) const
	{
		if (cfile) fprintf(cfile, "<?xml ");
		if (str)	 (*str) += "<?xml ";

		if (!version.empty()) {
			if (cfile) fprintf(cfile, "version=\"%s\" ", version.c_str());
			if (str) { (*str) += "version=\""; (*str) += version; (*str) += "\" "; }
		}
		if (!encoding.empty()) {
			if (cfile) fprintf(cfile, "encoding=\"%s\" ", encoding.c_str());
			if (str) { (*str) += "encoding=\""; (*str) += encoding; (*str) += "\" "; }
		}
		if (!standalone.empty()) {
			if (cfile) fprintf(cfile, "standalone=\"%s\" ", standalone.c_str());
			if (str) { (*str) += "standalone=\""; (*str) += standalone; (*str) += "\" "; }
		}
		if (cfile) fprintf(cfile, "?>");
		if (str)	 (*str) += "?>";
	}

	virtual void Print(FILE* cfile, int depth) const {
		Print(cfile, depth, 0);
	}

	virtual const char* Parse(const char* p, TiXmlParsingData* data, TiXmlEncoding encoding);

	/// <summary>
	/// Cast to a more defined type. Will return null not of the requested type.
	/// </summary>
	virtual const TiXmlDeclaration* ToDeclaration() const {
		return this;
	}

	/// <summary>
	/// Cast to a more defined type. Will return null not of the requested type.
	/// </summary>
	virtual TiXmlDeclaration*       ToDeclaration() {
		return this;
	}

	/// <summary>
	/// Walk the XML tree visiting this node and all of its children.
	/// </summary>
	virtual bool Accept(TiXmlVisitor* visitor) const {
		return visitor->Visit(*this);
	}
};
