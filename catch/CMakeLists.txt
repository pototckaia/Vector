cmake_minimum_required(VERSION 3.9)

project(catch)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_LIB test.cpp)

add_library(catch STATIC ${SOURCE_LIB})
