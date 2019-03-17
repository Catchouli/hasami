#pragma once

#include "renderer/renderer.hpp"
#include <memory>

namespace hs {
namespace gl {

class GLRenderer : public hs::Renderer
{
public:
  GLRenderer();
  hs::Shader* createShader() override;
  hs::Buffer* createBuffer() override;
  hs::Texture* createTexture() override;
  hs::StateManager* stateManager() override { return m_stateManager.get(); }
  void drawArrays(PrimitiveType prim, int start, int count) override;
  void drawIndexed(PrimitiveType prim, int start, int count, hs::IndexFormat indexFormat) override;
  void clear(bool color, bool depth) override;

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
