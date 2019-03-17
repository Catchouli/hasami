#pragma once

#include <vector>
#include <memory>
#include <matrix.hpp>
#include "renderer/renderer.hpp"
#include "renderer/standardmaterial.hpp"
#include "scene/scenegraph.hpp"
#include <optional>

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

  Mesh(Renderer& renderer, bool indexed = false);
  void loadObj(const char* path, Normals normals);
  bool loadCachedObj(const char* path, size_t versionHash);
  void writeCachedObj(const char* path, size_t versionHash, const std::vector<Vertex>& vbuf, const std::vector<Attrib>& attribs);
  void draw(Renderer& renderer, StandardMaterial& mat, const Context& ctx);

  bool m_indexed;
  int m_start, m_count; //^ for indexed mode
  IndexFormat m_indexFormat;

  std::shared_ptr<hs::Buffer> m_buf;
  std::shared_ptr<hs::Buffer> m_ibuf;
  std::vector<Attrib> m_attrib;
};

}
