cmake_minimum_required(VERSION 3.22)
project(MatrixWithChannel)

set(CMAKE_CXX_STANDARD 20)

add_executable(MatrixWithChannel main.cpp  Matrix.cpp Matrix.h BlockMatrix.cpp BlockMatrix.h BufferedChannel.h)
