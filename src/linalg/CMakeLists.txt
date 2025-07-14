cmake_minimum_required(VERSION 3.14)

project(LinAlg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(LinAlg types.cxx)
target_include_directories(LinAlg INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

include(CTest)

# Fetch Catch2 v3.x
include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.0 # pick the version you want
)
FetchContent_MakeAvailable(catch2)

add_executable(LinAlgTest types_test.cxx)
target_link_libraries(LinAlgTest PRIVATE LinAlg Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(LinAlgTest)