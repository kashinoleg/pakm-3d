cmake_minimum_required(VERSION 3.9)
project(TinyXML.Test)

set(SOURCE_LIB
	TinyXMLIntegration.cpp
)

add_library(TinyXML.Test SHARED
	${SOURCE_LIB}
)

include_directories(../)
target_link_libraries(TinyXML.Test TinyXML.Core)
