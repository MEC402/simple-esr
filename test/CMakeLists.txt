project(test_esr)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
endif(UNIX)

include_directories("catch" "../src/")

set(TEST_SOURCE_FILES tests.cpp test_main.cpp data.h)
add_executable(test_esr "../src/poopy.h" "../src/poopy.cpp" "${TEST_SOURCE_FILES}")
