cmake_minimum_required(VERSION 3.23)
project(Number)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-fPIC)

add_library(Number Number.cpp)
