#pragma once

/** The types of XML nodes supported by TinyXml. (All the
unsupported types are picked up by UNKNOWN.)
*/
enum TiXmlNodeType
{
	TINYXML_DOCUMENT,
	TINYXML_ELEMENT,
	TINYXML_COMMENT,
	TINYXML_UNKNOWN,
	TINYXML_TEXT,
	TINYXML_DECLARATION,
	TINYXML_TYPECOUNT
};
