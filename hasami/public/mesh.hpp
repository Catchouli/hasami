#pragma once

#include "buffer.hpp"
#include "shader.hpp"

#include <vector>
#include "optional.hpp"
#include "glad/glad.h"
#include "glm.hpp"

struct Attrib {
  Attrib(std::string name, GLint size, GLenum type, std::optional<int> offset = std::optional<int>())
    : name(name), size(size), type(type), offset(offset) {}

  std::string name;
  GLint size;
  GLenum type;
  std::optional<int> offset;
};

struct Vertex {
  glm::vec3 pos;
  glm::vec3 nrm;
  glm::vec2 texCoord;
};

class Mesh
{
public:
  Mesh();
  void loadObj(const char* path);
  bool loadCachedObj(const char* path);
  void writeCachedObj(const char* path, const std::vector<Vertex>& vbuf, const std::vector<Attrib>& attribs);
  void draw(const Shader& shader, const glm::mat4& projection, const glm::mat4& modelview);

  Buffer<float> m_buf;
  std::vector<Attrib> m_attrib;
};