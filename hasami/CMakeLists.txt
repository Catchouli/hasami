project(hasami VERSION 0.1 LANGUAGES CXX)

add_library(hasami
  public/constants.hpp
  public/renderer.hpp
  private/renderer.cpp
  public/renderstate.hpp
  public/scenegraph.hpp
  private/scenegraph.cpp
  public/globe.hpp
  private/globe.cpp
  public/camera.hpp
  private/camera.cpp
  public/mesh.hpp
  private/mesh.cpp
  public/material.hpp
  private/material.cpp
  public/standardmaterial.hpp
  public/filewatchservice.hpp
  private/filewatchservice.cpp
  public/gl/sdlwindow.hpp
  private/gl/sdlwindow.cpp
  public/gl/glbuffer.hpp
  private/gl/glbuffer.cpp
  public/gl/glshader.hpp
  private/gl/glshader.cpp
  public/gl/glrenderer.hpp
  private/gl/glrenderer.cpp
  public/gl/gltexture.hpp
  private/gl/gltexture.cpp
  public/util.hpp
)

target_link_libraries(hasami SDL2 glad glm SDL2 tiny_obj_loader FreeImage simplefilewatcher imgui)

target_include_directories(hasami PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:public>
    PRIVATE private)