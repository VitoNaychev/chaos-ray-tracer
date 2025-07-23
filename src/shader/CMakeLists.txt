cmake_minimum_required(VERSION 3.14)

project(Shader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Shader shader.cxx)
target_link_libraries(Shader PUBLIC Engine LinAlg Scene)
target_include_directories(Shader INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})