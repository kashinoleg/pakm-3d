cmake_minimum_required(VERSION 3.9)
project(Renderer.Python)

IF(BUILD_PYTHON_INTERFACE)
	# Build the renderer python module
	IF(BUILD_RENDERER)
		# Find out the dependencies for the python renderer module interface
		FILE(GLOB RENDERER_HEADER_FILES ../Renderer/*.h)

		FIND_PACKAGE(SWIG REQUIRED)
		ADD_CUSTOM_COMMAND(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/Renderer_wrap.cxx"
		COMMAND "${SWIG_EXECUTABLE}" -c++ -python -outdir "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/Renderer.i"
		MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/Renderer.i"
		DEPENDS ${RENDERER_HEADER_FILES}
		COMMENT "Generating SWIG wrapper code...")

		ADD_LIBRARY(Renderer.Python MODULE Renderer.i Renderer_wrap.cxx)

		SET_SOURCE_FILES_PROPERTIES(Renderer_wrap.cxx PROPERTIES GENERATED 1)

		set_target_properties(Renderer.Python PROPERTIES OUTPUT_NAME "_Renderer")
		set_target_properties(Renderer.Python PROPERTIES SUFFIX ".pyd")

		FIND_PACKAGE(VTK REQUIRED)
		INCLUDE(${VTK_USE_FILE})
		target_link_libraries(Renderer.Python ${VTK_LIBRARIES})

		FIND_PACKAGE(PythonLibs REQUIRED)
		include_directories(${PYTHON_INCLUDE_PATH})
		target_link_libraries(Renderer.Python ${PYTHON_LIBRARIES})


		TARGET_LINK_LIBRARIES(Renderer.Python Renderer.Core)

		INSTALL(FILES
			Renderer.py
		DESTINATION
			${CMAKE_INSTALL_PREFIX}/Python/libxtra/TexGen
		)

		INSTALL(TARGETS
			Renderer.Python LIBRARY
		DESTINATION
			${CMAKE_INSTALL_PREFIX}/Python/libxtra/TexGen
		)
	ENDIF(BUILD_RENDERER)
ENDIF(BUILD_PYTHON_INTERFACE)
