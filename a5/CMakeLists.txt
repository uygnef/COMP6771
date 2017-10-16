cmake_minimum_required(VERSION 3.8)
project(a5)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp BucketSort.cpp BucketSort.h)
add_executable(a5 ${SOURCE_FILES})