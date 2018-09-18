#pragma once
#include "TiXmlAttribute.hpp"

/*	A class used to manage a group of attributes.
It is only used internally, both by the ELEMENT and the DECLARATION.

The set can be changed transparent to the Element and Declaration
classes that use it, but NOT transparent to the Attribute
which has to implement a next() and previous() method. Which makes
it a bit problematic and prevents the use of STL.

This version is implemented with circular lists because:
- I like circular lists
- it demonstrates some independence from the (typical) doubly linked list.
*/
class TiXmlAttributeSet
{
public:
	TiXmlAttributeSet();
	~TiXmlAttributeSet();

	void Add(TiXmlAttribute* attribute);
	void Remove(TiXmlAttribute* attribute);

	const TiXmlAttribute* First()	const { return (sentinel.next == &sentinel) ? 0 : sentinel.next; }
	TiXmlAttribute* First() { return (sentinel.next == &sentinel) ? 0 : sentinel.next; }
	const TiXmlAttribute* Last() const { return (sentinel.prev == &sentinel) ? 0 : sentinel.prev; }
	TiXmlAttribute* Last() { return (sentinel.prev == &sentinel) ? 0 : sentinel.prev; }

	TiXmlAttribute*	Find(const char* _name) const;
	TiXmlAttribute* FindOrCreate(const char* _name);

	TiXmlAttribute*	Find(const std::string& _name) const;
	TiXmlAttribute* FindOrCreate(const std::string& _name);


private:
	//*ME:	Because of hidden/disabled copy-construktor in TiXmlAttribute (sentinel-element),
	//*ME:	this class must be also use a hidden/disabled copy-constructor !!!
	TiXmlAttributeSet(const TiXmlAttributeSet&);	// not allowed
	void operator=(const TiXmlAttributeSet&);	// not allowed (as TiXmlAttribute)

	TiXmlAttribute sentinel;
};
