#include "hikari.hpp"
#include "glad/glad.h"
#include "trace.hpp"

#include <SDL.h>
#include <memory>

struct Hikari::Impl
{
  GLuint m_fbo;
  GLuint m_colorBuf;
  std::shared_ptr<CudaSurface> m_cudaSurface;
};

Hikari::Hikari()
  : m_running(true)
  , m_impl(nullptr)
{
  m_impl = new Impl;

  glGenTextures(1, &m_impl->m_colorBuf);
  glBindTexture(GL_TEXTURE_2D, m_impl->m_colorBuf);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 800, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glGenFramebuffers(1, &m_impl->m_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_impl->m_colorBuf, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_fbo);
  glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  m_impl->m_cudaSurface = std::make_shared<CudaSurface>(m_impl->m_colorBuf);
}

Hikari::~Hikari()
{
  glDeleteTextures(1, &m_impl->m_colorBuf);
  delete m_impl;
}

void Hikari::input(const SDL_Event* evt)
{
}

void Hikari::render(hs::Window* window)
{
  const float time = SDL_GetTicks() / 1000.0f;

  // Clear buffer
  glBindFramebuffer(GL_FRAMEBUFFER, m_impl->m_fbo);
  glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Render to texture
  trace(m_impl->m_cudaSurface.get(), time);

  // blit buffer
  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_impl->m_fbo);
  glBlitFramebuffer(0, 0, 800, 800, 0, 0, 800, 800, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

  // check error
  cudaCheckErr();

  // Update fps counter
  m_framerate.m_frames++;
  if (time - m_framerate.m_lastUpdate > 1.0f) {
    m_framerate.m_lastUpdate = time;
    m_framerate.m_fps = m_framerate.m_frames;
    m_framerate.m_frames = 0;
    printf("fps: %d\n", m_framerate.m_fps);
  }
}
