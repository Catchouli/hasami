#pragma once

#include <vector>
#include <memory>
#include <matrix.hpp>
#include "optional.hpp"
#include "renderer.hpp"
#include "standardmaterial.hpp"

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
  enum class Normals { Provided, Flat, Smooth };

  Mesh(Renderer& renderer);
  void loadObj(const char* path, Normals normals);
  bool loadCachedObj(const char* path, size_t versionHash);
  void writeCachedObj(const char* path, size_t versionHash, const std::vector<Vertex>& vbuf, const std::vector<Attrib>& attribs);
  void draw(Renderer& renderer, StandardMaterial& mat, const glm::mat4& projection, const glm::mat4& model, const glm::mat4& obj);

  std::shared_ptr<hs::Buffer> m_buf;
  std::vector<Attrib> m_attrib;
};

}