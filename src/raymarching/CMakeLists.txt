cmake_minimum_required(VERSION 3.14)

project(RayMarching LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(RayMarching raymarching.cxx)
target_link_libraries(RayMarching PUBLIC Shader LinAlg Scene)
target_include_directories(RayMarching INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})