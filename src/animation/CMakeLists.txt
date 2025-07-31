cmake_minimum_required(VERSION 3.14)

project(Animation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Animation animation.cxx)
target_link_libraries(Animation PUBLIC Engine)
target_include_directories(Animation INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

include(CTest)

FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.0 # pick the version you want
)
FetchContent_MakeAvailable(catch2)

add_executable(AnimationTest animation_test.cxx)
target_link_libraries(AnimationTest PRIVATE Animation Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(AnimationTest)