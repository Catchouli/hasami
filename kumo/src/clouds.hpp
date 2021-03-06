#pragma once

#include "scene/scenegraph.hpp"

class CloudsNode
  : public hs::ModelNode
{
public:
  CloudsNode(hs::Renderer& renderer);

  virtual void draw(hs::Renderer& renderer, const hs::Context& ctx) override;
};