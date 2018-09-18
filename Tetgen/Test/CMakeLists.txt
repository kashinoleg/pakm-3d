cmake_minimum_required(VERSION 3.9)
project(Tetgen.Test)

set(SOURCE_LIB
	OctreeDimensions.cpp
)

add_library(Tetgen.Test SHARED
	${SOURCE_LIB}
)

include_directories(../)
target_link_libraries(Tetgen.Test Tetgen.Core)
