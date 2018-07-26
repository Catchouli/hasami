#pragma once

#include "backends/gl/renderer.hpp"
#include "glad/glad.h"
#include <string>
#include <map>
#include <tuple>
#include <functional>
#include <typeindex>
#include <thread>
#include <mutex>
#include <deque>
#include "util/optional.hpp"
#include <FileWatcher/FileWatcher.h>

namespace hs {
namespace gl {

struct CachedShader
{
  CachedShader(GLuint prog, size_t hash) : m_prog(prog), m_hash(hash), m_dirty(true), m_valid(false) {}

  GLuint m_prog;
  size_t m_hash;
  bool m_dirty;
  bool m_valid;
};

class Shader
  : public hs::Shader, public FW::FileWatchListener
{
public:
  Shader();
  ~Shader();

  /// Set the file path for this glsl shader (deferred load on bind)
  void load(const char* srcPath) override;

  /// Load a shader from disk - immediately loads and builds the shader unlike load()
  void loadFromFile(const char* srcPath);

  /// Whether this shader is valid
  bool valid() override { return m_cachedShader.has_value() && m_cachedShader.value()->m_valid; }

  /// Build a shader from source
  void build(const std::string& shaderSource);

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

  /// Set a uniform to be enabled or disabled
  void setUniformEnabled(const char* name, bool enabled) override;

  /// Handle file changes
  virtual void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action) override;

private:
  struct Attribute { int location; AttribType type; bool unused; };
  struct Uniform { int location; UniformType type; bool unused; bool enabled; };

  /// Whether *this shader* is dirty (not the underlying cached shader)
  /// This gets set whenever an attribute/uniform is added or enabled/disabled
  /// And indicates that the shader should be reloaded from source
  bool m_dirty;

  /// The filename for reloading
  std::string m_filepath;

  /// The filename only from the file path
  std::string m_filename;

  /// The shader program
  std::optional<CachedShader*> m_cachedShader;

  /// Next attribute location
  int m_nextAttribLocation;

  /// Next uniform location
  int m_nextUniformLocation;

  /// Vertex attributes
  std::map<std::string, Attribute> m_attribs;

  /// Shader vars
  std::map<std::string, Uniform> m_uniforms;
};

}
}