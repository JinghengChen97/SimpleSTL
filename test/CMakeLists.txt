cmake_minimum_required(VERSION 3.17)
project(alloc_tester)

set(CMAKE_CXX_STANDARD 11)
INCLUDE_DIRECTORIES(../SimpleSTL)

add_executable(${PROJECT_NAME}
        alloc_tester.cpp
        ../SimpleSTL/alloc.cpp)