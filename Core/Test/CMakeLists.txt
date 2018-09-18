cmake_minimum_required(VERSION 3.9)
project(Core.Test)

set(SOURCE_LIB
	OctreeDimensions.cpp
	
	FibreDistribTests.cpp
	
	GeometricTests.cpp
	
	MesherTests.cpp
	
	PropertyTests.cpp
	
	SolverTests.cpp
  
	TestUtilities.cpp
	TestUtilities.h
  
	TextileFactory.cpp
	TextileFactory.h
  
	VoxelExportTests.cpp
	
	XMLTests.cpp
)

add_library(Core.Test SHARED
	${SOURCE_LIB}
)

include_directories(../)
target_link_libraries(Core.Test Core.Core)
