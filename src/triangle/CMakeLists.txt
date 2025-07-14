cmake_minimum_required(VERSION 3.14)

project(triangle)

add_executable(Triangle main.cxx)
target_link_libraries(Triangle PRIVATE LinAlg PixelDrawer RayGen)
