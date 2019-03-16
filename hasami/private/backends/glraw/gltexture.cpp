#include "glad/glad.h"
#include "backends/glraw/gltexture.hpp"
#include "backends/gl/renderer.hpp"

namespace hs {
namespace gl {

Texture::Texture()
  : m_target(GL_TEXTURE_2D)
{
  glGenTextures(1, &m_id);
}

Texture::~Texture()
{
  glDeleteTextures(1, &m_id);
}

void Texture::set(TextureFormat f, int width, int height, int depth, void* ptr)
{
  m_target = depth > 1 ? GL_TEXTURE_3D : GL_TEXTURE_2D;
  bind(TextureUnit::Texture0);

  glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  GLint internalFormat = GL_RGBA;
  GLint format = GL_RGBA;
  GLint type = GL_UNSIGNED_BYTE;

  switch (f) {
    case TextureFormat::RGBA8888: internalFormat = GL_UNSIGNED_BYTE; format = GL_RGBA; type = GL_UNSIGNED_BYTE; break;
    case TextureFormat::R32F: internalFormat = GL_R32F; format = GL_RED; type = GL_FLOAT; break;
    default: assert(false); return;
  }

  if (depth > 1) {
    glTexImage3D(m_target, 0, internalFormat, width, height, depth, 0, format, type, ptr);
  }
  else {
    glTexImage2D(m_target, 0, internalFormat, width, height, 0, format, type, ptr);
  }
}

void Texture::bind(TextureUnit unit)
{
  glActiveTexture(GL_TEXTURE0 + (int)unit);
  glBindTexture(m_target, m_id);
}

}
}
