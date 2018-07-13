#include "gl/glbuffer.hpp"

namespace hs {
namespace gl {

GLenum glUsage(hs::BufferUsage usage) {
  // todo: support more
  return GL_STATIC_DRAW;
}

GLenum glTarget(hs::BufferTarget target) {
  // todo: support more
  if (target == hs::BufferTarget::VertexBuffer)
    return GL_ARRAY_BUFFER;
  else if (target == hs::BufferTarget::IndexBuffer)
    return GL_ELEMENT_ARRAY_BUFFER;

  // unimplemented
  assert(false);
  return GL_ARRAY_BUFFER;
}

Buffer::Buffer()
  : m_size(0)
  , m_stride(0)
{
  glGenBuffers(1, &m_buf);
}

Buffer::~Buffer()
{
  glDeleteBuffers(1, &m_buf);
}

void Buffer::set(const void* buf, int size, int stride, hs::BufferUsage usage)
{
  if (size <= 0)
    return;

  bind(hs::BufferTarget::VertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, size, buf, glUsage(usage));
  m_size = size;
  m_stride = stride;
}

void Buffer::bind(hs::BufferTarget target)
{
  // todo: use target
  glBindBuffer(glTarget(target), m_buf);
}

}
}