cmake_minimum_required(VERSION 3.14)

project(Engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Engine engine.cxx enginenew.cxx)
target_link_libraries(Engine PUBLIC LinAlg Scene RayGen Shader Tracer)
target_include_directories(Engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

include(CTest)

# Fetch Catch2 v3.x
include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.0 # pick the version you want
)
FetchContent_MakeAvailable(catch2)

add_executable(NewEngineTest engine_test.cxx)
target_link_libraries(NewEngineTest PRIVATE Engine LinAlg Scene Shader Tracer Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(NewEngineTest)


