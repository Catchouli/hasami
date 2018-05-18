#pragma once

#include "gl/buffer.hpp"
#include "gl/shader.hpp"

#include <vector>
#include <matrix.hpp>
#include "optional.hpp"

namespace hs {

struct Attrib {
  enum class Type { Float };

  Attrib(std::string name, size_t size, Type type, std::optional<int> offset = std::optional<int>())
    : name(name), size(size), type(type), offset(offset) {}

  std::string name;
  size_t size;
  Type type;
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
  void draw(gl::Shader& shader, const glm::mat4& projection, const glm::mat4& modelview);

  gl::Buffer<float> m_buf;
  std::vector<Attrib> m_attrib;
};

}