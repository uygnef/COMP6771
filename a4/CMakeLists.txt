cmake_minimum_required(VERSION 3.8)
project(a4)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES allTests.cpp btree.h btree_iterator.h)
add_executable(a4 ${SOURCE_FILES})