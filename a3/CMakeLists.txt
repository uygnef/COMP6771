cmake_minimum_required(VERSION 3.8)
project(a3)
add_definitions(-Wall)
add_definitions(-Werror)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp Graph.h Graph.tem)
add_executable(a3 ${SOURCE_FILES})