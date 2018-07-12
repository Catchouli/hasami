#pragma once

#include "renderstate.hpp"

#include <optional.hpp>
#include <matrix.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <vector>
#include <variant.hpp>

#include <map>
#include <stack>
#include <set>

// todo: abstract input
union SDL_Event;

namespace hs {

/// Renderer API
/// Example usage:
///   class MyApp : public hs::App;
///   hs::sdl::SDLWindow<hs::gl::GLRenderer> window(true);
///   MyApp app;
///   window.setApp(&app);
///   window.run();
///
///   window.renderer(); //^ access the renderer api
///
/// Everything else can be accessed through the Renderer

class App;
class Window;
class Renderer;
class Shader;
class Buffer;
class Texture;
class StateManager;

enum class UniformType { Float, Mat4, Sampler2D, Unknown };
enum class AttribType { Float, Vec2, Vec3, Vec4, Unknown };
enum class PrimitiveType { Triangles };
enum class TextureFormat { RGBA8888 };
enum class TextureUnit { Texture0, Texture1, Texture2, Texture3, Texture4, Texture5, Texture6, Texture7, Texture_Max };
enum class BufferTarget { VertexBuffer };
enum class BufferUsage { StaticDraw };

/// App interface
/// End users should implement this interface and pass it to a window
/// to update and service
class App {
public:
  virtual void render(Window* window) = 0;
  virtual bool running() = 0;
  virtual void input(const SDL_Event* evt) = 0;
};

/// Window interface
/// Implementers of this interface create a window and then service an app
/// calling its render function and providing it with input. A window also
/// provides its app with a Renderer.
class Window
{
public:
  virtual ~Window() {}

  // Interface

  virtual void setApp(App* app) = 0;
  virtual void run() = 0;
  virtual Renderer* renderer() = 0;
};

/// Renderer interface
/// Provides access to drawing functions and other renderer resources.
class Renderer
{
public:
  virtual ~Renderer() {}

  // Interface

  virtual Shader* createShader() = 0;
  virtual Buffer* createBuffer() = 0;
  virtual Texture* createTexture() = 0;
  virtual StateManager* stateManager() = 0;
  virtual void clear(bool color, bool depth) = 0;
  virtual void drawArrays(PrimitiveType prim, int start, int count) = 0;
  virtual bool checkError() = 0;
};

/// Shader interface
/// Allows a shader program to be loaded and bound, and allows attributes and
/// uniforms to be bound to the shader program. Uniforms and attributes added
/// before calling load() are automatically added to the shader source.
class Shader
{
public:
  virtual ~Shader() {}

  // Interface

  virtual void load(const char* srcPath) = 0;
  virtual bool valid() = 0;
  virtual void bind() = 0;
  virtual void unbind() = 0;
  virtual void addAttrib(const char* name, AttribType type) = 0;
  virtual void bindAttrib(const char* name, int size, AttribType type, size_t stride, int offset) = 0;
  virtual void unbindAttrib(const char* name) = 0;
  virtual void addUniform(const char* name, UniformType type) = 0;
  virtual void setUniform(const char* name, UniformType type, const void* buf) = 0;
  virtual void setUniformEnabled(const char* name, bool enabled) = 0;
};

/// Buffer interface
/// Used to provide binary data to the renderer, with some convenience functions
/// for initialization.
class Buffer {
public:
  template <typename T>
  void set(const std::vector<T>& buf, int stride, BufferUsage usage);

  template <typename T>
  void set(const T* buf, int size, int stride, BufferUsage usage);

  // Interface

  virtual void set(const void* buf, int size, int stride, BufferUsage usage) = 0;
  virtual void bind(BufferTarget target) = 0;
  virtual int stride() = 0;
  virtual int size() = 0;
};

/// A set function that takes a vector<T> and updates the buffer with its
/// contents
template <typename T>
void Buffer::set(const std::vector<T>& buf, int stride, BufferUsage usage)
{
  set((const void*)buf.data(), buf.size() * sizeof(T), stride, usage);
}

/// A set function that takes a typed array and its size and updates the
/// buffer with its contents
template <typename T>
void Buffer::set(const T* buf, int size, int stride, BufferUsage usage)
{
  set((const void*)buf, size * sizeof(T), stride, usage);
}

/// StateManager
/// The state manager allows the renderer state to be updated in a stack
/// so that the previous state can be restored when you're done rendering
class StateManager
{
public:
  StateManager();
  void pushState(RenderState renderState);
  void popState(RenderState renderState);
  void flush();

  // Interface

  virtual void applyState(const RenderState& renderState) = 0;

private:
  std::map<RenderState::State, std::stack<RenderState>> m_stacks;
  std::set<std::stack<RenderState>*> m_dirtyStates;
};

/// Texture
class Texture
{
public:
  /// Load a texture from disk
  bool load(const char* path);

  // Interface

  virtual void set(TextureFormat f, int width, int height, void* ptr) = 0;
  virtual void bind(TextureUnit unit) = 0;
};

}