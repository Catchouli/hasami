project(tiny_obj_loader VERSION 0.1 LANGUAGES CXX)

add_library(tiny_obj_loader
  impl.cpp
)

target_include_directories(tiny_obj_loader PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)