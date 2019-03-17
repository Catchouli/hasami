#pragma once
#include "backends/gl/renderer.hpp"
#include <functional>

class TestApp : public hs::App
{
public:
  TestApp(std::function<void(hs::Window*)> render, std::function<bool()> running)
    : m_render(render), m_running(running) {}

  TestApp(const TestApp&) = delete;
  TestApp& operator=(const TestApp&) = delete;

  virtual void render(hs::Window* window) { m_render(window); }
  virtual bool running() { return m_running(); }
  void input(const SDL_Event* evt) override { }

  std::function<void(hs::Window*)> m_render;
  std::function<bool()> m_running;
};

inline void withTestApp(hs::Window* window, std::function<void(hs::Window*)> func)
{
  bool updated = false;
  TestApp testApp([&updated, &func](auto* win) { func(win);  updated = true; }, [&updated]() { return !updated; });
  window->setApp(&testApp);
  window->run();
}