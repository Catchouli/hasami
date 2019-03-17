#pragma once

#include "scene/scenegraph.hpp"
#include "renderer/standardmaterial.hpp"
#include "renderer/mesh.hpp"

namespace hs {

class GlobeNode
  : public ModelNode
{
public:
  GlobeNode();

  virtual void draw(Renderer& renderer, const Context& ctx) override;

private:
  void generate(Renderer& renderer, const glm::vec3& center);
  void subdivide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& center, float size = 1.0f);
  void generateGlobe(const glm::vec3& center);

  std::vector<Vertex> m_vert;
};

}