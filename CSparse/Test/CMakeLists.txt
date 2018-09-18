cmake_minimum_required(VERSION 3.9)
project(CSparse.Test)

set(SOURCE_LIB
	OctreeDimensions.cpp
)

add_library(CSparse.Test SHARED
	${SOURCE_LIB}
)

include_directories(../)
target_link_libraries(CSparse.Test CSparse.Core)
