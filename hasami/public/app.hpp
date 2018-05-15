#pragma once

class Window;
union SDL_Event;

class App {
public:
  virtual void render(Window* window) = 0;
  virtual bool running() = 0;
  virtual void input(const SDL_Event* evt) = 0;
};