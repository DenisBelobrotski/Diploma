# CMakeList.txt : CMake project for Diploma, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# Add source to this project's executable.
add_executable (Diploma "Diploma.cpp" "Diploma.h" Plot.cpp Plot.h)

# TODO: Add tests and install targets if needed.
