project(hasami VERSION 0.1 LANGUAGES CXX)

add_library(hasami
  public/app.hpp
  private/app.cpp
  public/renderer.hpp
  private/renderer.cpp
  public/scenegraph.hpp
  private/scenegraph.cpp
  public/globe.hpp
  private/globe.cpp
  public/camera.hpp
  private/camera.cpp
  public/mesh.hpp
  private/mesh.cpp
  public/gl/sdlwindow.hpp
  private/gl/sdlwindow.cpp
  public/gl/buffer.hpp
  public/gl/shader.hpp
  private/gl/shader.cpp
)

target_link_libraries(hasami SDL2 glad glm SDL2 tiny_obj_loader)

target_include_directories(hasami PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:public>
    PRIVATE private)