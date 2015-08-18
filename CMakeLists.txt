cmake_minimum_required(VERSION 3.3)
project(esr)

if (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(UNIX)

include_directories("src/"  "test/catch/")


set(SOURCE_FILES 
	src/main.cpp 
	src/poopy.h 
	src/poopy.cpp)

add_executable(esr ${SOURCE_FILES})

set(TEST_SOURCE_FILES test/test_main.cpp)
add_executable(esr_test ${TEST_SOURCE_FILES})
