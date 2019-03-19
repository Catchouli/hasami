cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(kumo LANGUAGES CXX)

# This somehow helps it find dlls but I don't remember exactly why because I added it a long time ago
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BIINARY_DIR}/bin)

# Disable crt secure warnings on windows
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Enable imgui support
set(USE_IMGUI ON CACHE BOOL "")
if(USE_IMGUI)
  add_definitions(-DUSE_IMGUI)
endif(USE_IMGUI)

# Macro for enabling C++17 support on a project
macro(enableCXX17 proj)
  target_compile_options(${proj} PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-std:c++latest>)
endmacro(enableCXX17)

# third party libs
add_subdirectory(libs/sdl2)
add_subdirectory(libs/glad)
add_subdirectory(libs/glm)
add_subdirectory(libs/tiny_obj_loader)
add_subdirectory(libs/FreeImage)
add_subdirectory(libs/simplefilewatcher)
add_subdirectory(libs/imgui)
add_subdirectory(libs/stb)
add_subdirectory(libs/catch)

# our projects
add_subdirectory(hasami)
add_subdirectory(hasami_test)
add_subdirectory(hikari)
add_subdirectory(kumo)
add_subdirectory(kumo_test)