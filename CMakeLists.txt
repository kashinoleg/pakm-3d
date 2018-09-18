cmake_minimum_required(VERSION 3.9)
PROJECT(TexGen)

SET(REAL_OUTPUT_PATH ${TexGen_BINARY_DIR}/$(OutDir))

SET(BUILD_RENDERER ON CACHE BOOL "Enable rendering using VTK.")
SET(BUILD_PYTHON_INTERFACE ON CACHE BOOL "Enable the Python interface.")
SET(BUILD_GUI ON CACHE BOOL "Build the graphical user interface.")
SET(BUILD_CASCADE_EXPORT OFF CACHE BOOL "Build module to export geometry to Open CASCADE.")
SET(BUILD_SHARED ON CACHE BOOL "Build shared libraries")

IF(BUILD_GUI)
	IF(NOT BUILD_RENDERER)
		MESSAGE("The renderer is needed in order to build the graphical user interface.\nSet BUILD_RENDERER ON.")
	ENDIF(NOT BUILD_RENDERER)
	IF(NOT BUILD_PYTHON_INTERFACE)
		MESSAGE("The Python interface is needed in order to build the graphical user interface.\nSet BUILD_PYTHON_INTERFACE ON.")
	ENDIF(NOT BUILD_PYTHON_INTERFACE)
ENDIF(BUILD_GUI)

SUBDIRS(
  Core
  CSparse
  Octree
  Tetgen
  TinyXML
)

IF(BUILD_RENDERER)
	SUBDIRS(Renderer)
ENDIF(BUILD_RENDERER)

IF(BUILD_CASCADE_EXPORT)
	SUBDIRS(Export)
ENDIF(BUILD_CASCADE_EXPORT)

IF(BUILD_GUI)
	SUBDIRS(GUI)
ENDIF(BUILD_GUI)

SET(LIBRARY_OUTPUT_PATH ${TexGen_BINARY_DIR} CACHE INTERNAL "Single output directory for building all libraries.")
SET(EXECUTABLE_OUTPUT_PATH ${TexGen_BINARY_DIR} CACHE INTERNAL "Single output directory for building all executables.")

SET(TEXGEN_INCLUDE_DIRS ${TexGen_SOURCE_DIR})
SET(TEXGEN_LIBRARY_DIRS ${LIBRARY_OUTPUT_PATH})
SET(TEXGEN_BUILD_RENDERER ${BUILD_RENDERER})
