project(hasami VERSION 0.1 LANGUAGES CXX)

add_library(hasami
  #  sdl backend
  public/backends/sdl/sdlwindow.hpp
  private/backends/sdl/sdlwindow.cpp
  #  gl backend
  public/backends/gl/glbuffer.hpp
  private/backends/gl/glbuffer.cpp
  public/backends/gl/glrenderer.hpp
  private/backends/gl/glrenderer.cpp
  public/backends/gl/glshader.hpp
  private/backends/gl/glshader.cpp
  public/backends/gl/gltexture.hpp
  private/backends/gl/gltexture.cpp
  #  renderer
  public/renderer/constants.hpp
  public/renderer/material.hpp
  private/renderer/material.cpp
  public/renderer/mesh.hpp
  private/renderer/mesh.cpp
  public/renderer/renderer.hpp
  private/renderer/renderer.cpp
  public/renderer/renderstate.hpp
  public/renderer/standardmaterial.hpp
  #  scene
  public/scene/camera.hpp
  private/scene/camera.cpp
  public/scene/scenegraph.hpp
  private/scene/scenegraph.cpp
  #  util
  public/util/filewatchservice.hpp
  private/util/filewatchservice.cpp
  public/util/util.hpp
)

target_link_libraries(hasami SDL2 glad glm SDL2 tiny_obj_loader FreeImage simplefilewatcher imgui)

target_include_directories(hasami PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:public>
    PRIVATE private)