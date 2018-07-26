#pragma once

#include "backends/gl/renderer.hpp"
#include "backends/gl/mesh.hpp"
#include "backends/gl/standardmaterial.hpp"
#include "scene/scenegraph.hpp"
#include "scene/camera.hpp"

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
  bool m_wireframe;

  std::shared_ptr<hs::FPSCamera> m_camera;

  std::shared_ptr<hs::SceneNode> m_scenegraph;

  // Miku
  std::shared_ptr<hs::AssemblyNode> m_miku;
  std::shared_ptr<hs::ModelNode> m_mikuModel;

  // Globe
  std::shared_ptr<hs::AssemblyNode> m_globe;

  struct {
    float m_lastUpdate = 0.0f;
    int m_frames = 0;
    int m_fps = 0;
  } m_framerate;
};