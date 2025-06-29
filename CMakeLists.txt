cmake_minimum_required(VERSION 3.14)
project(YourProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
include(CTest)

# Fetch Catch2 v3.x
include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.0 # pick the version you want
)
FetchContent_MakeAvailable(catch2)

# Library source
add_library(core src/factorial.cpp src/ppm.cpp src/circle.cpp)

# Your application
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE core)

# Test target
add_executable(tests tests/main_test.cpp)
target_link_libraries(tests PRIVATE core Catch2::Catch2WithMain)

# Register tests with CTest
include(Catch)
catch_discover_tests(tests)