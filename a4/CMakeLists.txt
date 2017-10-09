cmake_minimum_required(VERSION 3.8)
project(a4)
add_definitions(-Wall)
add_definitions(-Werror)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES allTests.cpp btree.h btree_iterator.h mattTests.h cameronTests.cpp cseTests.cpp)
add_executable(a4 ${SOURCE_FILES})