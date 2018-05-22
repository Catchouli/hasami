#include "gl/sdlwindow.hpp"
#include "SDL.h"
#include <stdio.h>

namespace hs {
namespace sdl {
namespace internal {

SDLWindowBase::SDLWindowBase(bool createGLContext)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    fprintf(stderr, "SDL error: %s\n", SDL_GetError());
    throw;
  }

  if (SDL_CreateWindowAndRenderer(800, 800, SDL_WINDOW_OPENGL, &m_win, &m_renderer) < 0) {
    fprintf(stderr, "SDL error: %s\n", SDL_GetError());
    throw;
  }

  if (createGLContext) {
    SDL_GL_CreateContext(m_win);
  }
}

SDLWindowBase::~SDLWindowBase()
{
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_win);
}

void SDLWindowBase::run()
{
  while (m_app && m_app->running()) {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        setApp(nullptr);
      }
      if (evt.type == SDL_MOUSEBUTTONDOWN) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_SetWindowGrab(m_win, SDL_TRUE);
      }
      else if (m_app) {
        m_app->input(&evt);
      }
    }

    if (m_app) {
      m_app->render(this);
      SDL_GL_SwapWindow(m_win);
      if (auto* renderer = this->renderer())
        renderer->checkError();
    }
  }
}

}
}
}