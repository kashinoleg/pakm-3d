#pragma once
#include "TiXmlNode.hpp"

/// <summary>
/// Any tag that tinyXml doesn't recognize is saved as an unknown.It is a tag of text, but should not be modified.
/// It will be written back to the XML, unchanged, when the file is saved. DTD tags get thrown into TiXmlUnknowns.
/// </summary>
class TiXmlUnknown : public TiXmlNode {
public:
	TiXmlUnknown() : TiXmlNode(TiXmlNodeType::TINYXML_UNKNOWN) {}
	virtual ~TiXmlUnknown() {}

	TiXmlUnknown(const TiXmlUnknown& copy) : TiXmlNode(TiXmlNodeType::TINYXML_UNKNOWN) { copy.CopyTo(this); }
	TiXmlUnknown& operator=(const TiXmlUnknown& copy) { copy.CopyTo(this); return *this; }

	/// <summary>
	/// Creates a copy of this Unknown and returns it.
	/// </summary>
	virtual TiXmlNode* Clone() const;

	/// <summary>
	/// Print this Unknown to a FILE stream.
	/// </summary>
	virtual void Print(FILE* cfile, int depth) const;

	virtual const char* Parse(const char* p, TiXmlParsingData* data, TiXmlEncoding encoding);

	/// <summary>
	/// Cast to a more defined type. Will return null not of the requested type.
	/// </summary>
	virtual const TiXmlUnknown* ToUnknown() const {
		return this;
	}

	/// <summary>
	/// Cast to a more defined type. Will return null not of the requested type.
	/// </summary>
	virtual TiXmlUnknown* ToUnknown() {
		return this;
	}

	/// <summary>
	/// Walk the XML tree visiting this node and all of its children.
	/// </summary>
	virtual bool Accept(TiXmlVisitor* content) const;
protected:
	void CopyTo(TiXmlUnknown* target) const;
	virtual void StreamIn(std::istream * in, std::string * tag);
};
