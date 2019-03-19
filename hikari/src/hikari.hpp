#pragma once

#include "renderer/renderer.hpp"

class Hikari : public hs::App
{
public:
  Hikari();

  void render(hs::Window* window) override;
  bool running() override { return m_running; }
  void input(const SDL_Event* evt) override;

private:
  bool m_running;

  struct {
    float m_lastUpdate = 0.0f;
    int m_frames = 0;
    int m_fps = 0;
  } m_framerate;
};
