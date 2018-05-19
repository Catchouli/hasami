#pragma once

#include "../renderer.hpp"

namespace hs {
namespace gl {

class GLRenderer : public hs::Renderer
{
public:
  GLRenderer();
  hs::Shader* createShader() override;
  hs::Buffer* createBuffer() override;
  hs::StateManager* stateManager() override { return m_stateManager.get(); }
  void drawArrays(PrimitiveType prim, int start, int count) override;
  void clear(const glm::vec4& col, bool color, bool depth) override;
  bool checkError() override;

private:
  std::shared_ptr<hs::StateManager> m_stateManager;
};

class StateManager
  : public hs::StateManager
{
public:
  void applyState(const RenderState& renderState);
};

}
}