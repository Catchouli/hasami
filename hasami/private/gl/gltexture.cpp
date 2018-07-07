#include "gl/gltexture.hpp"

#include "glad/glad.h"

namespace hs {
namespace gl {

Texture::Texture()
  : m_target(GL_TEXTURE_2D)
{
  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
  glDeleteTextures(1, &m_id);
}

void Texture::set(TextureFormat f, int width, int height, void* ptr)
{
  m_target = GL_TEXTURE_2D;
  bind(TextureUnit::Texture0);

  switch (f) {
    case TextureFormat::RGBA8888: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ptr); return;
    default: assert(false); return;
  }
}

void Texture::bind(TextureUnit unit)
{
  glActiveTexture(GL_TEXTURE0 + (int)unit);
  glBindTexture(m_target, m_id);
}

}
}