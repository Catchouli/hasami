project(catch VERSION 0.1 LANGUAGES CXX)

add_library(catch
  catch.cpp
)

target_include_directories(catch PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)