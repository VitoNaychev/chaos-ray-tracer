cmake_minimum_required(VERSION 3.14)

project(Visualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Visualizer Visualizer.cpp)
target_include_directories(Visualizer PRIVATE ../)
target_include_directories(Visualizer PRIVATE ../pixeldrawer)
target_include_directories(Visualizer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
