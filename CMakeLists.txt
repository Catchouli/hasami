cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(hasami_dist VERSION 0.1 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BIINARY_DIR}/bin)

set(USE_IMGUI ON CACHE BOOL "")
if(USE_IMGUI)
  add_definitions(-DUSE_IMGUI)
endif(USE_IMGUI)

add_subdirectory(sdl2)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(tiny_obj_loader)
add_subdirectory(FreeImage)
add_subdirectory(simplefilewatcher)
add_subdirectory(imgui)
add_subdirectory(hasami)
add_subdirectory(demo)