cmake_minimum_required(VERSION 3.9)
project(GUI)

set(SOURCE_EXECUTABLE
	CircleRangeCtrl.cpp
	CircleRangeCtrl.h

	CompleteDialog.cpp
	CompleteDialog.h

	ControlsWindow.cpp
	ControlsWindow.h

	Convert.h

	CustomWidget.cpp
	CustomWidget.h

	DomainPlanesDialog.cpp
	DomainPlanesDialog.h

	DropTarget.cpp
	DropTarget.h

	EmbeddedPythonFunctions.cpp
	EmbeddedPythonFunctions.h

	latin_random.cpp
	latin_random.hpp

	LayersOffsetDialog.cpp
	LayersOffsetDialog.h

	LoggerGUI.h

	Modeller.cpp
	Modeller.h

	PatternDraftDialog.cpp
	PatternDraftDialog.h

	PythonConsole.cpp
	PythonConsole.h

	PythonConverter.cpp
	PythonConverter.h

	PythonWrapper.cpp
	PythonWrapper.h

	RangeValidator.cpp
	RangeValidator.h

	SectionSelect.cpp
	SectionSelect.h

	TexGenApp.cpp
	TexGenApp.h

	TexGenMainFrame.cpp
	TexGenMainFrame.h

	TexGenRenderWindow.cpp
	TexGenRenderWindow.h

	TextileLayersDialog.cpp
	TextileLayersDialog.h

	WeavePatternCtrl.cpp
	WeavePatternCtrl.h

	WeavePatternCtrl3D.cpp
	WeavePatternCtrl3D.h

	WeaveWizard.cpp
	WeaveWizard.h

	WeaveWizard3D.cpp
	WeaveWizard3D.h

	WindowIDs.h

	wxVTKRenderWindowInteractor.cxx
	wxVTKRenderWindowInteractor.h

	YarnSectionSelect.cpp
	YarnSectionSelect.h
)

SET(CMAKE_MODULE_PATH ${TexGen_SOURCE_DIR}/GUI ${CMAKE_MODULE_PATH})

ADD_EXECUTABLE(GUI
	WIN32
	${SOURCE_EXECUTABLE}
	Embedded.i
	GUI.rc
)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
target_link_libraries(GUI ${VTK_LIBRARIES})

SET(wxWidgets_USE_LIBS base core aui adv xrc xml html)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include_directories(${wxWidgets_INCLUDE_DIRS})
target_link_libraries(GUI ${wxWidgets_LIBRARIES})
ADD_DEFINITIONS(${wxWidgets_DEFINITIONS})
SET_TARGET_PROPERTIES(GUI PROPERTIES COMPILE_FLAGS "${wxWidgets_CXX_FLAGS}")

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})
target_link_libraries(GUI ${PYTHON_LIBRARIES})

FIND_PACKAGE(SWIG REQUIRED)
ADD_CUSTOM_COMMAND(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/Embedded_wrap.cxx"
COMMAND "${SWIG_EXECUTABLE}" -c++ -python -outdir "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/Embedded.i"
MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/Embedded.i"
DEPENDS "EmbeddedPythonFunctions.h"
COMMENT "Generating SWIG wrapper code...")

include_directories(../TinyXML)
include_directories(../Tetgen)
include_directories(../Core)
include_directories(../Renderer)
target_link_libraries(GUI Renderer.Core)

INSTALL(TARGETS
	GUI RUNTIME
DESTINATION
	${CMAKE_INSTALL_PREFIX}
)

INSTALL(FILES
	TexGen.xrc
DESTINATION
	${CMAKE_INSTALL_PREFIX}
)
