#pragma once

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "renderer.hpp"

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

private:
  SDL_Window* m_win;
  SDL_Renderer* m_renderer;
  SDL_GLContext m_context;

  App* m_app;
};

}
}