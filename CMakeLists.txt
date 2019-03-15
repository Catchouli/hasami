cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(kumo VERSION 0.1 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BIINARY_DIR}/bin)

# Enable imgui support
set(USE_IMGUI ON CACHE BOOL "")
if(USE_IMGUI)
  add_definitions(-DUSE_IMGUI)
endif(USE_IMGUI)

add_subdirectory(libs/sdl2)
add_subdirectory(libs/glad)
add_subdirectory(libs/glm)
add_subdirectory(libs/tiny_obj_loader)
add_subdirectory(libs/FreeImage)
add_subdirectory(libs/simplefilewatcher)
add_subdirectory(libs/imgui)

# Enable C++17 in VS2017
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()

add_subdirectory(hasami)
add_subdirectory(kumo)