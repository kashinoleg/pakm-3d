#pragma once
#include "TiXmlBase.hpp"
#include "TiXmlQueryType.hpp"

/// <summary>
/// An attribute is a name-value pair. Elements have an arbitrary number of attributes, each with a unique name.
/// The attributes are not TiXmlNodes, since they are not part of the tinyXML document object model.
/// There are other suggested ways to look at this problem.
/// </summary>
using namespace std;
class TiXmlAttribute : public TiXmlBase {
	friend class TiXmlAttributeSet;
private:
	/// <summary>
	/// A pointer back to a document, for error reporting.
	/// </summary>
	TiXmlDocument*	document;
	string name;
	string value;
	TiXmlAttribute*	prev;
	TiXmlAttribute*	next;
private:
	TiXmlAttribute(const TiXmlAttribute&) = delete;
	void operator=(const TiXmlAttribute& base) = delete;
public:
	/// <summary>
	/// Construct an empty attribute.
	/// </summary>
	TiXmlAttribute() : TiXmlBase() {
		document = NULL;
		prev = NULL;
		next = NULL;
	}

	/// <summary>
	/// String constructor.
	/// </summary>
	TiXmlAttribute(const string& setName, const string& setValue) {
		name = setName;
		value = setValue;
		document = NULL;
		prev = NULL;
		next = NULL;
	}

	/// <summary>
	/// Construct an attribute with a name and value.
	/// </summary>
	TiXmlAttribute(const char * _name, const char * _value) {
		name = _name;
		value = _value;
		document = NULL;
		prev = NULL;
		next = NULL;
	}

	/// <summary>
	/// Return the name of this attribute.
	/// </summary>
	const string& Name() const {
		return name;
	}

	/// <summary>
	/// Set the name of this attribute.
	/// </summary>
	void SetName(const string& setName) {
		name = setName;
	}

	/// <summary>
	/// Return the value of this attribute.
	/// </summary>
	const string& Value() const {
		return value;
	}

	/// <summary>
	/// Set the value.
	/// </summary>
	void SetValue(const string& setValue) {
		value = setValue;
	}

	/// <summary>
	/// Return the value of this attribute, converted to an integer.
	/// </summary>
	int IntValue() const {
		return stoi(value);
	}

	/// <summary>
	/// Return the value of this attribute, converted to a double.
	/// </summary>
	double DoubleValue() const {
		return stod(value);
	}

	/** QueryIntValue examines the value string. It is an alternative to the
	IntValue() method with richer error checking.
	If the value is an integer, it is stored in 'value' and
	the call returns TIXML_SUCCESS. If it is not
	an integer, it returns TIXML_WRONG_TYPE.

	A specialized but useful call. Note that for success it returns 0,
	which is the opposite of almost all other TinyXml calls.
	*/
	TiXmlQueryType QueryIntValue(int* ival) const {
		if (sscanf(value.c_str(), "%d", ival) == 1) {
			return TIXML_SUCCESS;
		}
		else {
			return TIXML_WRONG_TYPE;
		}
	}

	/// <summary>
	/// QueryDoubleValue examines the value string. See QueryIntValue().
	/// </summary>
	TiXmlQueryType QueryDoubleValue(double* dval) const {
		if (sscanf(value.c_str(), "%lf", dval) == 1) {
			return TIXML_SUCCESS;
		}
		else {
			return TIXML_WRONG_TYPE;
		}
	}

	/// <summary>
	/// Get the next sibling attribute in the DOM. Returns null at end.
	/// </summary>
	const TiXmlAttribute* Next() const {
		// We are using knowledge of the sentinel. The sentinel
		// have a value or name.
		if (next->value.empty() && next->name.empty()) {
			return 0;
		}
		else {
			return next;
		}
	}

	/// <summary>
	/// Get the next sibling attribute in the DOM. Returns null at end.
	/// </summary>
	TiXmlAttribute* Next() {
		return const_cast<TiXmlAttribute*>((const_cast<const TiXmlAttribute*>(this))->Next());
	}

	/// <summary>
	/// Get the previous sibling attribute in the DOM. Returns null at beginning.
	/// </summary>
	const TiXmlAttribute* Previous() const {
		// We are using knowledge of the sentinel. The sentinel
		// have a value or name.
		if (prev->value.empty() && prev->name.empty()) {
			return 0;
		}
		else {
			return prev;
		}
	}

	TiXmlAttribute* Previous() {
		return const_cast<TiXmlAttribute*>((const_cast<const TiXmlAttribute*>(this))->Previous());
	}

	bool operator==(const TiXmlAttribute& rhs) const {
		return name == rhs.name;
	}

	bool operator<(const TiXmlAttribute& rhs) const {
		return name < rhs.name;
	}

	bool operator>(const TiXmlAttribute& rhs) const {
		return name > rhs.name;
	}

	/// <summary>
	/// Attribute parsing starts: first letter of the name returns: the next char after the value end quote
	/// </summary>
	virtual const char* Parse(const char* p, TiXmlParsingData* data, TiXmlEncoding encoding);

	/// <summary>
	/// Prints this Attribute to a FILE stream.
	/// </summary>
	virtual void Print(FILE* cfile, int depth) const {
		Print(cfile, depth, NULL);
	}

	void Print(FILE* cfile, int /*depth*/, string* str) const {
		std::string n;
		std::string v;

		EncodeString(name, &n);
		EncodeString(value, &v);

		if (value.find('\"') == std::string::npos) {
			if (cfile) {
				fprintf(cfile, "%s=\"%s\"", n.c_str(), v.c_str());
			}
			if (str) {
				(*str) += n; (*str) += "=\""; (*str) += v; (*str) += "\"";
			}
		}
		else {
			if (cfile) {
				fprintf(cfile, "%s='%s'", n.c_str(), v.c_str());
			}
			if (str) {
				(*str) += n; (*str) += "='"; (*str) += v; (*str) += "'";
			}
		}
	}

	/// <summary>
	/// Set the document pointer so the attribute can report errors.
	/// </summary>
	void SetDocument(TiXmlDocument* doc) {
		document = doc;
	}
};
