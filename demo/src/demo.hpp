#pragma once

#include "renderer.hpp"
#include "app.hpp"
#include "window.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "scenegraph.hpp"

#include "glm.hpp"

#include "glad/glad.h"

#include "optional.hpp"

#include <memory>

class SceneNode;
class AssemblyNode;
class FPSCamera;

class Demo : public App
{
public:
  Demo();

  void render(Window* window) override;
  bool running() override { return m_running; }
  void input(const SDL_Event* evt);

private:
  bool m_running;

  std::shared_ptr<FPSCamera> m_camera;

  std::optional<Shader> m_shader;

  std::shared_ptr<SceneNode> m_scenegraph;
  std::shared_ptr<AssemblyNode> m_buddha;
  std::shared_ptr<AssemblyNode> m_globe;

  Buffer<float> m_buff;
  GLuint m_buf;
  GLsizei m_bufSize;
};