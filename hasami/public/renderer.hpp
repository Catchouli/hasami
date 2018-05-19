#pragma once

#include <functional>
#include <optional.hpp>
#include <matrix.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>

namespace hs {

class App;
class Renderer;

class Window
{
public:
  virtual ~Window() {}

  virtual void setApp(App* app) = 0;
  virtual void run() = 0;
};

class Renderer
{
public:
  virtual ~Renderer() {}
};

class Shader
{
public:
};

class RendererComponent
{
public:
  Renderer* renderer() const { return m_renderer; }

private:
  friend class Renderer;
  Renderer* m_renderer;
};

class ShaderVarBase
{
public:
  enum class Type { Mat4, Unknown };

  /// Buffer containing the shader var
  virtual void* buf() = 0;

  /// Size of the shader var
  virtual int size() = 0;

  /// Alignment of the shader var
  virtual int alignment() = 0;

  /// Whether the var is dirty
  bool dirty() const { return m_dirty; }

  /// Clears the dirty flag
  void clearDirty() { m_dirty = false; }

  /// Get the type of this var
  Type type() const { return m_type; }

  /// Called (if set) when the value changes
  std::function<void(ShaderVarBase&)> Dirty;

protected:
  /// Sets the dirty flag
  void setDirty() { m_dirty = true; }

  Type m_type = Type::Unknown;

private:
  /// Set when the value changes
  bool m_dirty = false;
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

  /// Buffer containing the shader var
  virtual void* buf() override { return &m_value; }

  /// Size of the shader var
  virtual int size() override { return sizeof(T); }

  /// Alignment of the shader var
  virtual int alignment() override;

private:

  /// The value of the shadervar
  /// todo: default values for types
  T m_value;
};

template <typename T>
int ShaderVar<T>::alignment()
{
  return 1;
}

template <>
inline int ShaderVar<glm::mat4>::alignment()
{
  return 4*4*sizeof(float);
}

template <>
inline int ShaderVar<glm::mat3>::alignment()
{
  return 4*4*sizeof(float);
}

template <typename T>
using VarRefT = std::tuple<std::string, ShaderVar<T>*>;

template <typename T>
constexpr VarRefT<T> VarRef(const std::string& str, ShaderVar<T>& v) { return VarRefT<T>(str, &v); }

}