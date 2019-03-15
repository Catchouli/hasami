project(imgui VERSION 0.1 LANGUAGES CXX)

add_library(imgui
  imgui.h
  imgui.cpp
  imconfig.h
  imgui_internal.h
  imgui_draw.cpp
  stb_rect_pack.h
  stb_textedit.h
  stb_truetype.h
)

target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)