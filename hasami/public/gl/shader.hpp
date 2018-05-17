#pragma once

#include "glad/glad.h"

namespace hs {
namespace gl {

class Shader
{
public:
  Shader(const char* srcPath);

  void load(const char* srcPath);

  GLuint prog() const { return m_prog; }

private:
  GLuint m_prog;
};

}
}