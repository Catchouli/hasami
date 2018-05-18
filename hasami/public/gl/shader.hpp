#pragma once

#include "glad/glad.h"
#include <map>
#include <tuple>
#include <functional>

namespace hs {
namespace gl {

class ShaderVarBase
{
  /// Buffer containing the shader var
  virtual void* buf() = 0;

  /// Size of the shader var
  virtual int size() = 0;

  /// Alignment of the shader var
  virtual int alignment() = 0;

  /// Called (if set) when the value changes
  std::function<void(ShaderVarBase&)> Dirty;
};

template <typename T>
class ShaderVar
  : public ShaderVarBase
{
public:
  /// Set the value of the shadervar
  void set(const T& value) { m_value = value; setDirty(); }

  /// Get the value of the shadervar
  const T& get() const { return m_value; }

  /// Clears the dirty flag
  void clearDirty() { m_dirty = false; }

  /// Buffer containing the shader var
  virtual void* buf() override { return &m_value; }

  /// Size of the shader var
  virtual int size() override { return 0; }

  /// Alignment of the shader var
  virtual int alignment() override { return 0; }

private:
  /// Sets the dirty flag
  void setDirty() { m_dirty = true; }

  /// The value of the shadervar
  T m_value;

  /// Set when the value changes
  bool m_dirty;
};

template <typename T>
using VarRefT = std::tuple<std::string, ShaderVar<T>&>;

template <typename T>
constexpr VarRefT<T> VarRef(const std::string& str, ShaderVar<T>& v) { return VarRefT<T>(str, v); }

class Shader
{
public:
  /// Initialize this shader by compiling a file
  Shader(const char* srcPath);

  /// Initialise this shader by compiling a file and adding shader vars
  template <typename... ShaderVars>
  Shader(const char* srcPath, VarRefT<ShaderVars>... shaderVars);

  /// Load a glsl shader
  void load(const char* srcPath);

  /// Bind the shader
  void bind();

  /// Flush shadervars
  void flush();

  /// Get the program id (deprecated)
  GLuint prog() const { return m_prog; }

private:
  /// Add shader vars (plumbing for addVar)
  void Shader::addVars() {}
  template <typename ShaderVar, typename... ShaderVars>
  void addVars(VarRefT<ShaderVar> var, VarRefT<ShaderVars>... rest);

  /// Add shader var
  template <typename ShaderVar>
  void addVar(VarRefT<ShaderVar> var);

  /// The shader program
  GLuint m_prog;

  /// Shader vars
  std::map<std::string, ShaderVarBase&> m_shaderVars;

  /// Read all shader uniforms attributes etc ids and store them
  void readUniforms();
  std::map<std::string, int> m_attributes;
  std::map<std::string, int> m_uniforms;
  std::map<std::string, int> m_uniformBlocks;
};

template <typename... ShaderVars>
Shader::Shader(const char* srcPath, VarRefT<ShaderVars>... shaderVars)
{
  load(srcPath);
  addVars(shaderVars...);
}

template <typename ShaderVar, typename... ShaderVars>
void Shader::addVars(VarRefT<ShaderVar> var, VarRefT<ShaderVars>... rest)
{
  addVar(var);
  addVars(rest...);
}

template <typename ShaderVar>
void Shader::addVar(VarRefT<ShaderVar> var)
{
  const std::string& name = std::get<0>(var);
  const auto& shadervar = std::get<1>(var);
  printf("Adding shader var: %s\n", name.c_str());
  m_shaderVars.insert(std::make_pair(name, shadervar));
}

}
}