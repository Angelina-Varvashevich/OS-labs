cmake_minimum_required(VERSION 3.23)
project(Vector)

set(CMAKE_CXX_STANDARD 20)
set(FIND_NUMBER_PATHS ${CMAKE_SOURCE_DIR}/../)

add_library(Vector SHARED Vector.cpp)

find_path(NUMBER_INCLUDE_DIR Number.h
        PATH_SUFFIXES Number
        PATHS ${FIND_NUMBER_PATHS})

include_directories(${NUMBER_INCLUDE_DIR})

find_library(NUMBER_LIBRARY
        NAMES Number
        PATH_SUFFIXES cmake-build-debug
        PATHS ${FIND_NUMBER_PATHS}/Number)


target_link_libraries(${PROJECT_NAME} ${NUMBER_LIBRARY})