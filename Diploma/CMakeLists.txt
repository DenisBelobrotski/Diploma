# CMakeList.txt : CMake project for Diploma, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

SET(GCC_ADDITIONAL_COMPILER_FLAGS "-O3")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_COMPILER_FLAGS}")

file(GLOB_RECURSE proj_sources *.cpp)

add_executable (Diploma ${proj_sources})
