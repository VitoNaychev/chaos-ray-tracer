cmake_minimum_required(VERSION 3.14)

project(Tracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Tracer tracer.cxx interx.cxx aabb.cxx)
target_link_libraries(Tracer PUBLIC Shader LinAlg Scene)
target_include_directories(Tracer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

include(CTest)

# Fetch Catch2 v3.x
include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.0 # pick the version you want
)
FetchContent_MakeAvailable(catch2)

add_executable(TracerTest interx_test.cxx)
target_link_libraries(TracerTest PRIVATE Shader Tracer LinAlg Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(TracerTest)