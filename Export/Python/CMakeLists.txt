cmake_minimum_required(VERSION 3.9)
project(Export.Python)

IF(BUILD_PYTHON_INTERFACE)
	# Build the renderer python module
	IF(BUILD_CASCADE_EXPORT)
		SET(EXPORT_HEADER_FILES ../Exporter.h)

		FIND_PACKAGE(SWIG REQUIRED)
		ADD_CUSTOM_COMMAND(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/Export_wrap.cxx"
		COMMAND "${SWIG_EXECUTABLE}" -c++ -python -outdir "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/Export.i"
		MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/Export.i"
		DEPENDS ${EXPORT_HEADER_FILES}
		COMMENT "Generating SWIG wrapper code...")

		ADD_LIBRARY(Export.Python MODULE Export.i Export_wrap.cxx)
		
		SET_SOURCE_FILES_PROPERTIES(Export_wrap.cxx PROPERTIES GENERATED 1)

		set_target_properties(Export.Python PROPERTIES OUTPUT_NAME "_Export")
		set_target_properties(Export.Python PROPERTIES SUFFIX ".pyd")

		FIND_PACKAGE(PythonLibs REQUIRED)
		include_directories(${PYTHON_INCLUDE_PATH})
		target_link_libraries(Export.Python ${PYTHON_LIBRARIES})

		TARGET_LINK_LIBRARIES(Export.Python Export.Core)

		INCLUDE_DIRECTORIES(${OPENCASCADE_INCLUDE_DIR})
		ADD_DEFINITIONS(-DWNT)
		TARGET_LINK_LIBRARIES(Export.Python
			${OPENCASCADE_STATIC_LIB}
		)

		INSTALL(FILES
			Export.py
		DESTINATION
			${CMAKE_INSTALL_PREFIX}/Python/libxtra/TexGen
		)
		INSTALL(TARGETS
			Export.Python LIBRARY
		DESTINATION
			${CMAKE_INSTALL_PREFIX}/Python/libxtra/TexGen
		)
	ENDIF(BUILD_CASCADE_EXPORT)
ENDIF(BUILD_PYTHON_INTERFACE)
