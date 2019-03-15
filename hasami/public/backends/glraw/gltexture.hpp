#pragma once
#include "backends/gl/renderer.hpp"

namespace hs {
namespace gl {

class Texture
  : public hs::Texture
{
public:
  Texture();
  ~Texture();

  virtual void set(TextureFormat f, int width, int height, void* ptr) override;
  virtual void bind(TextureUnit unit) override;

private:
  unsigned int m_id;
  unsigned int m_target;
};

}
}
