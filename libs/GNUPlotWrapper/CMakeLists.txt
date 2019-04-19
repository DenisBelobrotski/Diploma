cmake_minimum_required(VERSION 3.13)

project(GNUPlotWrapper VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE proj_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(GNUPlotWrapper STATIC ${proj_sources})
