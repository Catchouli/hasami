#pragma once

#include <vector>
#include <memory>
#include <matrix.hpp>
#include "optional.hpp"
#include "renderer.hpp"

namespace hs {

struct Attrib {
  Attrib(std::string name, size_t size, AttribType type, std::optional<int> offset = std::optional<int>())
    : name(name), size(size), type(type), offset(offset) {}

  std::string name;
  size_t size;
  AttribType type;
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
  Mesh(Renderer& renderer);
  void loadObj(const char* path);
  bool loadCachedObj(const char* path);
  void writeCachedObj(const char* path, const std::vector<Vertex>& vbuf, const std::vector<Attrib>& attribs);
  void draw(Renderer& renderer, Shader& shader, const glm::mat4& projection, const glm::mat4& modelview);

  std::shared_ptr<hs::Buffer> m_buf;
  std::vector<Attrib> m_attrib;
};

}