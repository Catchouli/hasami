#pragma once

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include <memory>
#include "renderer.hpp"

namespace hs {
class App;
}

namespace hs {
namespace sdl {

namespace internal {

class SDLWindowBase : public hs::Window {
public:
  SDLWindowBase(bool createGLContext);
  ~SDLWindowBase();

  void setApp(App* app) override { m_app = app; }

  void run() override;

private:
  SDL_Window* m_win;
  SDL_Renderer* m_renderer;

  App* m_app;
};

}

template <typename T>
class Window
  : public internal::SDLWindowBase
{
public:
  Window(bool createGLContext) : internal::SDLWindowBase(createGLContext) {}
  hs::Renderer* renderer() override { return m_rendererImpl.get(); }

private:
  std::shared_ptr<T> m_rendererImpl = std::make_shared<T>();
};

}
}