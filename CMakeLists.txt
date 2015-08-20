cmake_minimum_required(VERSION 3.3)
project(esr)

if (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
endif(UNIX)


set(SOURCE_FILES 
	src/poopy.h
	src/poopy.cpp)

add_executable(esr "src/main.cpp" ${SOURCE_FILES})

add_subdirectory("test/")
