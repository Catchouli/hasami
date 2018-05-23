#pragma once

#include "renderer.hpp"
#include "glad/glad.h"
#include <map>
#include <tuple>
#include <functional>
#include <typeindex>
#include <thread>

namespace hs {
namespace gl {

class Shader
  : public hs::Shader
{
public:
  Shader();
  ~Shader();

  /// Load a glsl shader
  void load(const char* srcPath) override;

  /// Generate the header for this shader
  std::string genHeader();

  /// Bind the shader
  void bind() override;

  /// Unbind the shader
  void unbind() override;

  /// Add an attribute
  void addAttrib(const char* name, AttribType type) override;

  /// Bind a vertex attribute
  void bindAttrib(const char* name, int size, AttribType type, size_t stride, int offset) override;

  /// Unbind a vertex attribute
  void unbindAttrib(const char* name) override;

  /// Add a uniform
  void addUniform(const char* name, UniformType type) override;

  /// Set a uniform value
  void setUniform(const char* name, UniformType type, const void* buf) override;

  /// Get the program id (deprecated)
  GLuint prog() const { return m_prog; }

  /// Start the shader watch thread
  static void startShaderWatchThread();

  /// The shader watch thread kernel
  static void shaderWatchThread();

private:
  struct Attribute { int location; AttribType type; bool unused; };
  struct Uniform { int location; UniformType type; bool unused; };

  /// Whether we built (sucessfully or just yet)
  bool m_valid;

  /// The shader program
  GLuint m_prog;

  /// Next attribute location
  int m_nextAttribLocation;

  /// Next uniform location
  int m_nextUniformLocation;

  /// Vertex attributes
  std::map<std::string, Attribute> m_attribs;

  /// Shader vars
  std::map<std::string, Uniform> m_uniforms;

  /// The file watch thread
  static std::thread sm_fileWatchThread;
};

}
}