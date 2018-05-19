#pragma once

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include <memory>
#include "gl/renderer.hpp"

namespace hs {
class App;
}

namespace hs {
namespace sdl {

class Window : public hs::Window {
public:
  Window(App* app = nullptr);
  ~Window();

  void setApp(App* app) override { m_app = app; }

  void run() override;

  hs::Renderer* renderer() override { return m_glRenderer.get(); }

private:
  SDL_Window* m_win;
  SDL_Renderer* m_renderer;
  SDL_GLContext m_context;

  std::shared_ptr<gl::GLRenderer> m_glRenderer;

  App* m_app;
};

}
}