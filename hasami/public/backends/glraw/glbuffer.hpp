#pragma once

#include "glad/glad.h"
#include <vector>
#include "backends/gl/renderer.hpp"

namespace hs {
  enum class BufferTarget;
  enum class BufferUsage;
}

namespace hs {
namespace gl {

class Buffer
  : public hs::Buffer
{
public:
  Buffer();
  ~Buffer();

  void set(const void* buf, int size, int stride, hs::BufferUsage usage) override;
  void bind(hs::BufferTarget target) override;

  int size() override { return m_size; }
  int stride() override { return m_stride; }

private:
  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;

  GLuint m_buf;
  GLsizei m_size;
  GLsizei m_stride;
};

}
}
