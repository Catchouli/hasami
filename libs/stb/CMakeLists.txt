project(stb VERSION 0.1 LANGUAGES CXX)

add_library(stb
  stb.cpp
)

target_include_directories(stb PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)