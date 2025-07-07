cmake_minimum_required(VERSION 3.14)

project(Types LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Types types.cpp)
target_include_directories(Types INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})