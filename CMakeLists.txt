cmake_minimum_required(VERSION 3.14)

project(ChaosCamp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

# Fetch Catch2 v3.x
include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.0 # pick the version you want
)
FetchContent_MakeAvailable(catch2)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/pixeldrawer)
# add_subdirectory(${CMAKE_SOURCE_DIR}/src/visualizer)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/raygen)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/parser)

# add_subdirectory(${CMAKE_SOURCE_DIR}/src/triangle)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/cmd)

