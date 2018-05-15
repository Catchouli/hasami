project(hasami VERSION 0.1 LANGUAGES CXX)

add_library(hasami
  public/app.hpp
  public/window.hpp
  public/renderer.hpp
  public/shader.hpp
  public/mesh.hpp
  public/buffer.hpp
  private/app.cpp
  private/window.cpp
  private/renderer.cpp
  private/shader.cpp
  private/mesh.cpp
)

target_link_libraries(hasami SDL2 glad glm SDL2 tiny_obj_loader)

target_include_directories(hasami PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    $<INSTALL_INTERFACE:public>
    PRIVATE private)