#pragma once

#include "renderer.hpp"
#include "mesh.hpp"
#include "scenegraph.hpp"
#include "camera.hpp"

#include <memory>

class Demo : public hs::App
{
public:
  Demo(hs::Window* window);

  void render(hs::Window* window) override;
  bool running() override { return m_running; }
  void input(const SDL_Event* evt);

private:
  bool m_running;

  std::shared_ptr<hs::FPSCamera> m_camera;

  std::shared_ptr<hs::Shader> m_shader;

  std::shared_ptr<hs::SceneNode> m_scenegraph;
  std::shared_ptr<hs::AssemblyNode> m_buddha;
  std::shared_ptr<hs::AssemblyNode> m_orbitPivot;
  std::shared_ptr<hs::AssemblyNode> m_orbitMiku;
  std::shared_ptr<hs::AssemblyNode> m_globe;
};