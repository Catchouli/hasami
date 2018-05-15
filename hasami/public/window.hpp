#pragma once

#define SDL_MAIN_HANDLED
#include "SDL.h"

class App;

class Window {
public:
  Window(App* app = nullptr);
  ~Window();

  void setApp(App* app) { m_app = app; }

  void run();

private:
  SDL_Window* m_win;
  SDL_Renderer* m_renderer;
  SDL_GLContext m_context;

  App* m_app;
};