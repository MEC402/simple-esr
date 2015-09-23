cmake_minimum_required(VERSION 2.8)

if (UNIX)
    #set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
    #set(CMAKE_AR "/usr/bin/llvm-ar")
    #set(CMAKE_LINKER "/usr/bin/llvm-ld")
    #set(CMAKE_NM "/usr/bin/llvm-nm")
    #set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
    #set(CMAKE_RANLIB "/usr/bin/llvm-ranlib")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-missing-braces")
endif(UNIX)

project(esr)

include_directories("src/tclap/include" )

set(SOURCE_FILES
    src/axis.h
    src/boundingvolume.h
    src/cmdline.h
    src/cmdline.cpp
    src/node.h
	src/svt.h
	src/svt.cpp
    src/vec3.h )

add_executable(esr "src/main.cpp" ${SOURCE_FILES})

#target_link_libraries(esr ${CRUFT_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

add_subdirectory("test/")
