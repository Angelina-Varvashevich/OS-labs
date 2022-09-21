cmake_minimum_required(VERSION 3.23)
project(Client)

set(CMAKE_CXX_STANDARD 20)
set(FIND_NUMBER_PATHS ${CMAKE_SOURCE_DIR}/../)
set(FIND_VECTOR_PATHS ${CMAKE_SOURCE_DIR}/../)

find_path(NUMBER_INCLUDE_DIR Number.h
        PATH_SUFFIXES Number
        PATHS ${FIND_NUMBER_PATHS})

find_path(VECTOR_INCLUDE_DIR Vector.h
        PATH_SUFFIXES Vector
        PATHS ${FIND_VECTOR_PATHS})

find_library(NUMBER_LIBRARY
        NAMES Number
        PATH_SUFFIXES cmake-build-debug
        PATHS ${FIND_NUMBER_PATHS}/Number)

find_library(VECTOR_LIBRARY
        NAMES Vector
        PATH_SUFFIXES cmake-build-debug
        PATHS ${FIND_VECTOR_PATHS}/Vector)


include_directories(${NUMBER_INCLUDE_DIR})
include_directories(${VECTOR_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${NUMBER_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${VECTOR_LIBRARY})



