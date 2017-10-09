cmake_minimum_required(VERSION 3.8)
project(a4)
add_definitions(-Wall)
add_definitions(-Werror)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES test01.cpp btree.h btree_iterator.h)
add_executable(a4 ${SOURCE_FILES})