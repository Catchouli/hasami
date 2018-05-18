#pragma once

#include "glad/glad.h"
#include <map>

namespace hs {
namespace gl {

class Shader
{
public:
  Shader(const char* srcPath);

  void load(const char* srcPath);

  GLuint prog() const { return m_prog; }

private:
  void readUniforms();

  GLuint m_prog;

  std::map<std::string, int> m_attributes;
  std::map<std::string, int> m_uniforms;
  std::map<std::string, int> m_uniformBlocks;
};

}
}