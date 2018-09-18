cmake_minimum_required(VERSION 3.9)
project(Export.Test)

set(SOURCE_LIB
	OctreeDimensions.cpp
)

add_library(Export.Test SHARED
	${SOURCE_LIB}
)

include_directories(../)
target_link_libraries(Export.Test Export.Core)
