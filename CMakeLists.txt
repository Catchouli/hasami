cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(hasami_dist VERSION 0.1 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BIINARY_DIR}/bin)

add_subdirectory(sdl2)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(tiny_obj_loader)
add_subdirectory(FreeImage)
add_subdirectory(hasami)
add_subdirectory(demo)