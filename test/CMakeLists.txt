cmake_minimum_required(VERSION 3.17)
project(TESTER)

set(CMAKE_CXX_STANDARD 11)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

INCLUDE_DIRECTORIES(../SimpleSTL
        ${GTEST_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}_ALLOC
        alloc_tester.cpp
        ../SimpleSTL/alloc.cpp)
target_link_libraries(${PROJECT_NAME}_ALLOC
        ${GTEST_BOTH_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})
