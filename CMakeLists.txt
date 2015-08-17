cmake_minimum_required(VERSION 3.3)
project(poopypants)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp poopy.h poopy.cpp)
add_executable(poopypants ${SOURCE_FILES})