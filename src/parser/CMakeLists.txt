cmake_minimum_required(VERSION 3.14)

project(Parser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  rapidjson
  GIT_REPOSITORY https://github.com/Tencent/rapidjson
  GIT_TAG master
)
FetchContent_Populate(rapidjson)

add_library(Parser crtscene.cxx)
target_link_libraries(Parser Types)
target_include_directories(Parser PUBLIC ${rapidjson_SOURCE_DIR}/include inte)
target_include_directories(Parser INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

include(CTest)

FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.0 # pick the version you want
)
FetchContent_MakeAvailable(catch2)

add_executable(ParserTest crtscene_test.cxx)
target_link_libraries(ParserTest PRIVATE Parser Types Catch2::Catch2WithMain)
target_include_directories(ParserTest PRIVATE ${rapidjson_SOURCE_DIR}/include)

include(Catch)
catch_discover_tests(ParserTest)