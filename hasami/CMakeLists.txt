project(hasami VERSION 0.1 LANGUAGES CXX)

add_library(hasami
  # backends
  #  sdl
  public/backends/sdl/sdlwindow.hpp
  private/backends/sdl/sdlwindow.cpp
  #  glraw
  public/backends/glraw/glbuffer.hpp
  private/backends/glraw/glbuffer.cpp
  public/backends/glraw/glrenderer.hpp
  private/backends/glraw/glrenderer.cpp
  public/backends/glraw/glshader.hpp
  private/backends/glraw/glshader.cpp
  public/backends/glraw/gltexture.hpp
  private/backends/glraw/gltexture.cpp
  #  gl renderer
  public/backends/gl/constants.hpp
  public/backends/gl/material.hpp
  private/backends/gl/material.cpp
  public/backends/gl/mesh.hpp
  private/backends/gl/mesh.cpp
  public/backends/gl/renderer.hpp
  private/backends/gl/renderer.cpp
  public/backends/gl/renderstate.hpp
  public/backends/gl/standardmaterial.hpp
  #  scene
  public/backends/scene/camera.hpp
  private/backends/scene/camera.cpp
  public/backends/scene/scenegraph.hpp
  private/backends/scene/scenegraph.cpp
  #  util
  public/backends/util/filewatchservice.hpp
  private/backends/util/filewatchservice.cpp
  public/backends/util/optional.hpp
  public/backends/util/variant.hpp
  public/backends/util/util.hpp
)

target_link_libraries(hasami SDL2 glad glm SDL2 tiny_obj_loader FreeImage simplefilewatcher imgui)

target_include_directories(hasami PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:public>
    PRIVATE private)