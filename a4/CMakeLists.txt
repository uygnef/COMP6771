cmake_minimum_required(VERSION 3.8)
project(a4)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp btree.h)
add_executable(a4 ${SOURCE_FILES})