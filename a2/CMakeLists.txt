cmake_minimum_required(VERSION 3.8)
project(a2)
add_definitions(-Wall)
add_definitions(-Werror)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp EuclideanVector.cpp EuclideanVector.h)
add_executable(a2 ${SOURCE_FILES})