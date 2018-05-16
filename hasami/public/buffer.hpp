#pragma once

#include "glad/glad.h"
#include <vector>

template <typename T>
class Buffer {
public:
  Buffer();
  ~Buffer();

  void set(const std::vector<T>& buf, GLsizei stride, GLenum usage);
  void set(const T* buf, GLsizei size, GLsizei stride, GLenum usage);

  void bind(GLenum target);

  GLsizei size() const { return m_size; }
  GLsizei stride() const { return m_stride; }

private:
  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;

  GLuint m_buf;
  GLsizei m_size;
  GLsizei m_stride;
};

template <typename T>
Buffer<T>::Buffer()
  : m_size(0)
  , m_stride(0)
{
  glGenBuffers(1, &m_buf);
}

template <typename T>
Buffer<T>::~Buffer()
{
  glDeleteBuffers(1, &m_buf);
}

template <typename T>
void Buffer<T>::set(const std::vector<T>& buf, GLsizei stride, GLenum usage)
{
  set(buf.data(), buf.size(), usage);
  m_stride = stride;
}

template <typename T>
void Buffer<T>::set(const T* buf, GLsizei size, GLsizei stride, GLenum usage)
{
  if (size <= 0)
    return;

  bind(GL_ARRAY_BUFFER);
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), buf, usage);
  m_size = size;
  m_stride = stride;
}

template <typename T>
void Buffer<T>::bind(GLenum target)
{
  glBindBuffer(target, m_buf);
}