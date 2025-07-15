cmake_minimum_required(VERSION 3.14)

project(renderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(renderer main.cxx)
target_link_libraries(renderer PRIVATE Engine Parser Scene LinAlg PixelDrawer )
