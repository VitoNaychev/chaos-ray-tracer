cmake_minimum_required(VERSION 3.14)

project(Animation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Header-only library for animation types
add_library(Animation INTERFACE)
target_include_directories(Animation INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Optional: specify the header files for IDE organization
# target_sources(Animation INTERFACE 
#     ${CMAKE_CURRENT_SOURCE_DIR}/animation.hxx
# )
