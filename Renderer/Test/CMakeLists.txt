cmake_minimum_required(VERSION 3.9)
project(Renderer.Test)

set(SOURCE_LIB
	OctreeDimensions.cpp
  ProfileTests.cpp
  
  TextileFactory.cpp
  TextileFactory.h
)

add_library(Renderer.Test SHARED
	${SOURCE_LIB}
)

include_directories(../)
target_link_libraries(Renderer.Test Renderer.Core)
