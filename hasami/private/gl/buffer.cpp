#include "gl/buffer.hpp"

namespace hs {
namespace gl {

GLenum glUsage(Buffer::Usage usage) {
  // todo: support more
  return GL_STATIC_DRAW;
}

GLenum glTarget(Buffer::Target target) {
  // todo: support more
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

void Buffer::set(const void* buf, int size, int stride, Buffer::Usage usage)
{
  if (size <= 0)
    return;

  bind(Buffer::Target::VertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, size, buf, glUsage(usage));
  m_size = size;
  m_stride = stride;
}

void Buffer::bind(Buffer::Target target)
{
  // todo: use target
  glBindBuffer(glTarget(target), m_buf);
}

}
}