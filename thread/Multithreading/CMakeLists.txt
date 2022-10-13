cmake_minimum_required(VERSION 3.22)
project(Multithreading)

set(CMAKE_CXX_STANDARD 20)

add_executable(Multithreading main.cpp Matrix.cpp Matrix.h BlockMatrix.cpp BlockMatrix.h)

set(CMAKE_CXX_FLAGS "-O3")