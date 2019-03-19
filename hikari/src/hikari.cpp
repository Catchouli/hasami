#include "hikari.hpp"

#include <SDL.h>

int trace();

Hikari::Hikari()
  : m_running(true)
{
  trace();
}

void Hikari::input(const SDL_Event* evt)
{
}

void Hikari::render(hs::Window* window)
{
  const float time = SDL_GetTicks() / 1000.0f;

  // Update fps counter
  m_framerate.m_frames++;
  if (time - m_framerate.m_lastUpdate > 1.0f) {
    m_framerate.m_lastUpdate = time;
    m_framerate.m_fps = m_framerate.m_frames;
    m_framerate.m_frames = 0;
  }
}
