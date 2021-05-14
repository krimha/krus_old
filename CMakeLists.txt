cmake_minimum_required(VERSION 3.20)

project(ParseLib)

set(CMAKE_CXX_STANDARD 17)

add_library(parselib 
    src/finite_state_machine.cpp

    include/parselib/finite_state_machine.h
    )
target_include_directories(parselib PUBLIC include)

add_subdirectory(lib/Catch2)

# Compile a file that handles compilation of the catch header
add_library(test_main tests/test_main.cpp)
target_link_libraries(test_main PRIVATE Catch2::Catch2)


add_executable(test_prog tests/compiled_tests.cpp)
target_link_libraries(test_prog
    PUBLIC
    test_main
    Catch2::Catch2
    parselib
    )


include(CTest)
include(lib/Catch2/contrib/Catch.cmake)
catch_discover_tests(test_prog)
