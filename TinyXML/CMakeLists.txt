cmake_minimum_required(VERSION 3.9)
project(TinyXML.Core)

set(SOURCE_LIB
	tinyxml.cpp
	tinyxml.h

	tinyxmlerror.cpp
	tinyxmlparser.cpp


	TiXmlCursor.hpp
	TiXmlEncoding.hpp
	TiXmlQueryType.hpp
	TiXmlEntity.hpp
	TiXmlNodeType.hpp
	TiXmlError.hpp

	TiXmlVisitor.hpp
	TiXmlBase.hpp
	TiXmlNode.hpp
	TiXmlAttribute.hpp
	TiXmlAttributeSet.hpp
	TiXmlElement.hpp
	TiXmlComment.hpp
	TiXmlText.hpp
	TiXmlDeclaration.hpp
	TiXmlUnknown.hpp
	TiXmlDocument.hpp
	TiXmlHandle.hpp
	TiXmlPrinter.hpp
)

add_library(TinyXML.Core STATIC
	${SOURCE_LIB}
)

add_subdirectory(Test)
