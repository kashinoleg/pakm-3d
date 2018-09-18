cmake_minimum_required(VERSION 3.9)
project(Core.Python)

IF(BUILD_PYTHON_INTERFACE)
	

	# Find out the dependencies for the python core module interface
	FILE(GLOB CORE_HEADER_FILES ../*.h)

	# Find where SWIG and Python libraries are installed
	FIND_PACKAGE(SWIG REQUIRED)
	ADD_CUSTOM_COMMAND(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/Core_wrap.cxx"
	COMMAND "${SWIG_EXECUTABLE}" -c++ -python -outdir "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/Core.i"
	MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/Core.i"
	DEPENDS ${CORE_HEADER_FILES}
	COMMENT "Generating SWIG wrapper code...")

	# Create the python core interface library
	ADD_LIBRARY(Core.Python MODULE Core.i Core_wrap.cxx)

	SET_SOURCE_FILES_PROPERTIES(Core_wrap.cxx PROPERTIES GENERATED 1)

	set_target_properties(Core.Python PROPERTIES OUTPUT_NAME "_Core")
	set_target_properties(Core.Python PROPERTIES SUFFIX ".pyd")


	include_directories(../../Tetgen)
	include_directories(../../Octree)
	include_directories(../../TinyXML)
	target_link_libraries(Core.Python Core.Core)

	FIND_PACKAGE(PythonLibs REQUIRED)
	include_directories(${PYTHON_INCLUDE_PATH})
	target_link_libraries(Core.Python ${PYTHON_LIBRARIES})
	
	SET(PYTHON_MODULES "'Abaqus', 'Ansys', 'Completer', 'FlowTex', 'GridFile', 'TexGenv2', 'WiseTex'")
	IF(BUILD_RENDERER)
		SET(PYTHON_MODULES "${PYTHON_MODULES}, 'Renderer'")
	ENDIF(BUILD_RENDERER)
	IF(BUILD_CASCADE_EXPORT)
		SET(PYTHON_MODULES "${PYTHON_MODULES}, 'Export'")
	ENDIF(BUILD_CASCADE_EXPORT)
	FILE(WRITE "__init__.py" "# __all__ = [${PYTHON_MODULES}]")

	# Find the TexGen python modules
	FILE(GLOB PYTHON_MODULE_FILES Lib/*.py)

	# Find the sample python scripts
	FILE(GLOB PYTHON_SAMPLE_SCRIPTS Scripts/*.py)

  INSTALL(FILES
    __init__.py
    Core.py
    ${PYTHON_MODULE_FILES}
  DESTINATION
    ${CMAKE_INSTALL_PREFIX}/Python/libxtra/TexGen
  )

  INSTALL(TARGETS
    Core.Python LIBRARY
  DESTINATION
    ${CMAKE_INSTALL_PREFIX}/Python/libxtra/TexGen
  )

  INSTALL(FILES
    ${PYTHON_SAMPLE_SCRIPTS}
  DESTINATION
    ${CMAKE_INSTALL_PREFIX}/Scripts
  )
ENDIF(BUILD_PYTHON_INTERFACE)
