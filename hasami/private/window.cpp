#include "window.hpp"
#include "app.hpp"
#include "SDL.h"
#include <stdio.h>
#include "glad/glad.h"

Window::Window(App* app)
  : m_app(app)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    fprintf(stderr, "SDL error: %s\n", SDL_GetError());
    throw;
  }

  if (SDL_CreateWindowAndRenderer(800, 800, SDL_WINDOW_OPENGL, &m_win, &m_renderer) < 0) {
    fprintf(stderr, "SDL error: %s\n", SDL_GetError());
    throw;
  }

  m_context = SDL_GL_CreateContext(m_win);

  if (!gladLoadGL()) {
    fprintf(stderr, "Glad (opengl) failed to initialise\n");
    throw;
  }

  SDL_SetRelativeMouseMode(SDL_TRUE);
  SDL_SetWindowGrab(m_win, SDL_TRUE);
}

Window::~Window()
{
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_win);
}

void Window::run()
{
  while (m_app && m_app->running()) {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        setApp(nullptr);
      }
      else if (m_app) {
        m_app->input(&evt);
      }
    }

    if (m_app) {
      m_app->render(this);
      SDL_GL_SwapWindow(m_win);
    }
  }
}