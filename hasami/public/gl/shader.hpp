#pragma once

#include "renderer.hpp"
#include "glad/glad.h"
#include <map>
#include <tuple>
#include <functional>
#include <typeindex>

namespace hs {
namespace gl {

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

  /// Get a typed shader var
  template <typename T>
  ShaderVar<T>* getShaderVar(const std::string& name);


private:
  /// Dirty our shader vars
  void dirtyVars() { m_varsDirty = true; }

  /// Add shader vars (plumbing for addVar)
  void Shader::addVars() {}
  template <typename ShaderVar, typename... ShaderVars>
  void addVars(VarRefT<ShaderVar> var, VarRefT<ShaderVars>... rest);

  /// Add shader var
  template <typename ShaderVar>
  void addVar(VarRefT<ShaderVar> var);

  /// The shader program
  GLuint m_prog;

  /// Whether any of our shader vars are dirty
  bool m_varsDirty;

  /// Shader vars
  std::map<std::string, ShaderVarBase*> m_shaderVars;
  std::map<std::type_index, std::map<std::string, void*>> m_typedShaderVars;

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
  auto& typedMap = m_typedShaderVars[typeid(ShaderVar)];
  const std::string& name = std::get<0>(var);
  auto shaderVar = std::get<1>(var);
  auto dirtyVars = std::bind(&Shader::dirtyVars, this);
  m_shaderVars.insert(std::make_pair(name, shaderVar));
  typedMap.insert(std::make_pair(name, shaderVar));
  printf("Adding shader var: %s\n", name.c_str());
}

template <typename T>
ShaderVar<T>* Shader::getShaderVar(const std::string& name)
{
  auto& map = m_typedShaderVars[typeid(T)];
  auto it = map.find(name);
  return it != map.end() ? static_cast<ShaderVar<T>*>(it->second) : nullptr;
}

}
}