project(hasami VERSION 0.1 LANGUAGES CXX)

add_library(hasami
  public/renderer.hpp
  private/renderer.cpp
)

target_link_libraries(hasami SDL2)

target_include_directories(hasami PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:public>
    PRIVATE private)