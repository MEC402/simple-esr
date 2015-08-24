cmake_minimum_required(VERSION 3.3)
project(esr)

if (UNIX)
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
    set(CMAKE_AR "/usr/bin/llvm-ar")
    set(CMAKE_LINKER "/usr/bin/llvm-ld")
    set(CMAKE_NM "/usr/bin/llvm-nm")
    set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
    set(CMAKE_RANLIB "/usr/bin/llvm-ranlib")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-missing-braces")
endif(UNIX)

if(NOT DEFINED CRUFT_INCLUDE_DIR)
    set(CRUFT_INCLUDE_DIR "")
endif()
set(CRUFT_INCLUDE_DIR ${${CRUFT_INCLUDE_DIR}} CACHE PATH "Path to cruft include dir.")

if(NOT DEFINED CRUFT_LIBRARIES)
    set(CRUFT_LIBRARIES "")
endif()
set(CRUFT_LIBRARIES ${${CRUFT_LIBRARIES}} CACHE FILEPATH "Path to cruft static library file.")

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories("src/tclap/include"
    "${CRUFT_INCLUDE_DIR}"
    "${GLFW_INCLUDE_DIRS}"
    "${GLEW_INCLUDE_DIRS}"
    )

set(SOURCE_FILES 
    src/cmdline.h
    src/cmdline.cpp
	src/poopy.h
	src/poopy.cpp)

add_executable(esr "src/main.cpp" ${SOURCE_FILES})

target_link_libraries(esr ${CRUFT_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

add_subdirectory("test/")
