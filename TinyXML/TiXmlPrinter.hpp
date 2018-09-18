#pragma once
#include <string>
#include "TiXmlVisitor.hpp"

/** Print to memory functionality. The TiXmlPrinter is useful when you need to:

-# Print to memory (especially in non-STL mode)
-# Control formatting (line endings, etc.)

When constructed, the TiXmlPrinter is in its default "pretty printing" mode.
Before calling Accept() you can call methods to control the printing
of the XML document. After TiXmlNode::Accept() is called, the printed document can
be accessed via the CStr(), Str(), and Size() methods.

TiXmlPrinter uses the Visitor API.
@verbatim
TiXmlPrinter printer;
printer.SetIndent( "\t" );

doc.Accept( &printer );
fprintf( stdout, "%s", printer.CStr() );
@endverbatim
*/
using namespace std;
class TiXmlPrinter : public TiXmlVisitor {
private:
	int depth;
	bool simpleTextPrint;
	string buffer;
	string indent;
	string lineBreak;
private:
	void DoIndent() {
		for (int i = 0; i < depth; ++i) {
			buffer += indent;
		}
	}
	void DoLineBreak() {
		buffer += lineBreak;
	}
public:
	TiXmlPrinter() : depth(0), simpleTextPrint(false), buffer(), indent("    "), lineBreak("\n") {}

	virtual bool VisitExit(const TiXmlDocument&) {
		return true;
	}

	virtual bool VisitEnter(const TiXmlDocument&) {
		return true;
	}

	virtual bool VisitEnter(const TiXmlElement& element, const TiXmlAttribute* firstAttribute) {
		DoIndent();
		buffer += "<";
		buffer += element.Value();

		for (const TiXmlAttribute* attrib = firstAttribute; attrib; attrib = attrib->Next())
		{
			buffer += " ";
			attrib->Print(0, 0, &buffer);
		}

		if (!element.FirstChild())
		{
			buffer += " />";
			DoLineBreak();
		}
		else
		{
			buffer += ">";
			if (element.FirstChild()->ToText()
				&& element.LastChild() == element.FirstChild()
				&& element.FirstChild()->ToText()->CDATA() == false)
			{
				simpleTextPrint = true;
				// no DoLineBreak()!
			}
			else
			{
				DoLineBreak();
			}
		}
		++depth;
		return true;
	}

	virtual bool VisitExit(const TiXmlElement& element) {
		--depth;
		if (!element.FirstChild())
		{
			// nothing.
		}
		else
		{
			if (simpleTextPrint)
			{
				simpleTextPrint = false;
			}
			else
			{
				DoIndent();
			}
			buffer += "</";
			buffer += element.Value();
			buffer += ">";
			DoLineBreak();
		}
		return true;
	}

	virtual bool Visit(const TiXmlDeclaration& declaration) {
		DoIndent();
		declaration.Print(0, 0, &buffer);
		DoLineBreak();
		return true;
	}

	virtual bool Visit(const TiXmlText& text) {
		if (text.CDATA()) {
			DoIndent();
			buffer += "<![CDATA[";
			buffer += text.Value();
			buffer += "]]>";
			DoLineBreak();
		}
		else if (simpleTextPrint) {
			std::string str;
			TiXmlBase::EncodeString(text.ValueTStr(), &str);
			buffer += str;
		}
		else {
			DoIndent();
			std::string str;
			TiXmlBase::EncodeString(text.ValueTStr(), &str);
			buffer += str;
			DoLineBreak();
		}
		return true;
	}

	virtual bool Visit(const TiXmlComment& comment) {
		DoIndent();
		buffer += "<!--";
		buffer += comment.Value();
		buffer += "-->";
		DoLineBreak();
		return true;
	}

	virtual bool Visit(const TiXmlUnknown& unknown) {
		DoIndent();
		buffer += "<";
		buffer += unknown.Value();
		buffer += ">";
		DoLineBreak();
		return true;
	}

	/** Set the indent characters for printing. By default 4 spaces
	but tab (\t) is also useful, or null/empty string for no indentation.
	*/
	void SetIndent(const char* _indent) { indent = _indent ? _indent : ""; }
	/// Query the indention string.
	const char* Indent() { return indent.c_str(); }
	/** Set the line breaking string. By default set to newline (\n).
	Some operating systems prefer other characters, or can be
	set to the null/empty string for no indenation.
	*/
	void SetLineBreak(const char* _lineBreak) { lineBreak = _lineBreak ? _lineBreak : ""; }
	/// Query the current line breaking string.
	const char* LineBreak() { return lineBreak.c_str(); }

	/** Switch over to "stream printing" which is the most dense formatting without
	linebreaks. Common when the XML is needed for network transmission.
	*/
	void SetStreamPrinting() {
		indent = "";
		lineBreak = "";
	}
	/// Return the result.
	const char* CStr() { return buffer.c_str(); }
	/// Return the length of the result string.
	size_t Size() { return buffer.size(); }
	/// Return the result.
	const std::string& Str() { return buffer; }
};
