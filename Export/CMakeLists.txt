cmake_minimum_required(VERSION 3.9)
project(Export.Core)

set(SOURCE_LIB
	Exporter.cpp
	Exporter.hxx

	Convert.cpp
	Convert.hpp
)

ADD_LIBRARY(Export.Core STATIC
	${SOURCE_LIB}
)

include_directories(../TinyXML)
include_directories(../Tetgen)
include_directories(../Core)
target_link_libraries(Export.Core Core.Core)

ADD_DEFINITIONS(-DWNT)
INCLUDE(FindOpenCascade.cmake)
include_directories(${OPENCASCADE_INCLUDE_DIR})
target_link_libraries(Export.Core
  ${OPENCASCADE_STATIC_LIB}
)

INSTALL(FILES
  ${OPENCASCADE_SHARED_LIB}
DESTINATION
  ${CMAKE_INSTALL_PREFIX}/Python/libxtra/TexGen
)

add_subdirectory(Test)
add_subdirectory(Python)
