project(glm VERSION 0.1 LANGUAGES CXX)

add_library(glm
  dummy.cpp
)

target_include_directories(glm PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glm>)