#pragma once

#include <functional>
#include <optional.hpp>
#include <matrix.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <vector>
#include <variant.hpp>
#include <map>
#include <stack>

namespace hs {

class App;
class Renderer;
class Shader;
class Buffer;
class StateManager;

enum class UniformType { Float, Mat4, Unknown };
enum class AttribType { Float, Vec3, Vec4, Unknown };
enum class PrimitiveType { Triangles };

struct RenderState
{
  enum class State { ClearColor, DepthTest, CullFace, PolygonMode };
  enum class PolygonMode { Point, Line, Fill };
  using Value = std::variant<glm::vec4, bool, PolygonMode>;

  State m_state;
  Value m_val;
};

inline RenderState ClearColor() { return RenderState{RenderState::State::ClearColor}; };
inline RenderState ClearColor(const glm::vec4& v) { return RenderState{RenderState::State::ClearColor, v}; };
inline RenderState DepthTest() { return RenderState{RenderState::State::DepthTest}; };
inline RenderState DepthTest(bool b) { return RenderState{RenderState::State::DepthTest, b}; };
inline RenderState CullFace() { return RenderState{RenderState::State::CullFace}; };
inline RenderState CullFace(bool b) { return RenderState{RenderState::State::CullFace, b}; };
inline RenderState PolygonMode() { return RenderState{RenderState::State::PolygonMode}; };
inline RenderState PolygonMode(RenderState::PolygonMode mode) { return RenderState{RenderState::State::PolygonMode, mode}; };

class Window
{
public:
  virtual ~Window() {}
  virtual void setApp(App* app) = 0;
  virtual void run() = 0;
  virtual Renderer* renderer() = 0;
};

class Renderer
{
public:
  virtual ~Renderer() {}
  virtual Shader* createShader() = 0;
  virtual Buffer* createBuffer() = 0;
  virtual StateManager* stateManager() = 0;
  virtual void clear(const glm::vec4& col, bool color, bool depth) = 0;
  virtual void drawArrays(PrimitiveType prim, int start, int count) = 0;
  virtual bool checkError() = 0;
};

class Shader
{
public:
  virtual ~Shader() {}
  virtual void load(const char* srcPath) = 0;
  virtual void bind() = 0;
  virtual void unbind() = 0;
  virtual void addAttrib(const char* name, AttribType type) = 0;
  virtual void bindAttrib(const char* name, int size, AttribType type, size_t stride, int offset) = 0;
  virtual void unbindAttrib(const char* name) = 0;
  virtual void addUniform(const char* name, UniformType type) = 0;
  virtual void setUniform(const char* name, UniformType type, const void* buf) = 0;
};

class Buffer {
public:
  enum class Target { VertexBuffer };
  enum class Usage { StaticDraw };

  template <typename T>
  void set(const std::vector<T>& buf, int stride, Usage usage);

  template <typename T>
  void set(const T* buf, int size, int stride, Usage usage);

  virtual void set(const void* buf, int size, int stride, Usage usage) = 0;
  virtual void bind(Target target) = 0;
  virtual int stride() = 0;
  virtual int size() = 0;
};

template <typename T>
void Buffer::set(const std::vector<T>& buf, int stride, Usage usage)
{
  set((const void*)buf.data(), buf.size() * sizeof(T), stride, usage);
}

template <typename T>
void Buffer::set(const T* buf, int size, int stride, Usage usage)
{
  set((const void*)buf, size * sizeof(T), stride, usage);
}

class StateManager
{
public:
  void pushInitialStates(); //^ call from derived class
  void pushState(RenderState renderState);
  void popState(RenderState renderState);

  virtual void applyState(const RenderState& renderState) = 0;

private:
  std::map<RenderState::State, std::stack<RenderState>> m_stacks;
};

inline void StateManager::pushInitialStates()
{
  pushState(DepthTest(false));
  pushState(CullFace(false));
  pushState(ClearColor(glm::vec4(0.0f)));
  pushState(PolygonMode(RenderState::PolygonMode::Fill));
}

inline void StateManager::pushState(RenderState renderState)
{
  auto& stack = m_stacks[renderState.m_state];
  stack.push(renderState);
  applyState(stack.top());
}

inline void StateManager::popState(RenderState renderState)
{
  auto& stack = m_stacks[renderState.m_state];
  stack.pop();
  if (!stack.empty())
    applyState(stack.top());
}

}