#pragma once

#include "glad/glad.h"
#include <vector>
#include "../renderer.hpp"

namespace hs {
namespace gl {

class Buffer
  : public hs::Buffer
{
public:
  Buffer();
  ~Buffer();

  void set(const void* buf, int size, int stride, Buffer::Usage usage) override;
  void bind(Buffer::Target target) override;

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