#include <stdio.h>
#include <fstream>
#include <sstream>
#include <sstream>
#include <regex>
#include <chrono>
#include "glad/glad.h"
#include <cassert>
#include "renderer/renderer.hpp"
#include "backends/gl/glshader.hpp"
#include "util/filewatchservice.hpp"
#include "shader_error_page.hpp"

#define LOG_UNUSED_SYMBOLS 0

namespace hs {
namespace gl {

int glSize(GLenum type) {
  switch (type) {
    case GL_BYTE: return 1;
    case GL_UNSIGNED_BYTE: return 1;
    case GL_SHORT: return 2;
    case GL_UNSIGNED_SHORT: return 2;
    case GL_INT: return 4;
    case GL_UNSIGNED_INT: return 4;
    case GL_HALF_FLOAT: return 2;
    case GL_FLOAT: return 4;
    case GL_DOUBLE: return 8;
    default: assert(false); return 0;
  }
}

GLenum uniformGlType(UniformType type) {
  switch (type) {
    case UniformType::Float: return GL_FLOAT;
    case UniformType::Mat4: assert(false); return static_cast<GLenum>(-1);
    default: assert(false); return static_cast<GLenum>(-1);
  }
}

GLenum attribGlType(AttribType type) {
  switch (type) {
    case AttribType::Float: return GL_FLOAT;
    default: assert(false); return static_cast<GLenum>(-1);
  }
}

std::string dirnameOf(const std::string& fname)
{
  size_t pos = fname.find_last_of("\\/");
  return (std::string::npos == pos) ? "" : fname.substr(0, pos);
}

std::string filenameOf(const std::string& fname)
{
  size_t pos = fname.find_last_of("\\/");
  return (std::string::npos == pos || pos >= fname.size()) ? fname : fname.substr(pos+1);
}

Shader::Shader()
  : m_nextAttribLocation(0)
  , m_nextUniformLocation(0)
  , m_dirty(false)
{
}

struct CachedShaderInternal
  : public CachedShader
{
  CachedShaderInternal(GLuint prog, size_t hash) : CachedShader(prog, hash) {}
  std::set<const Shader*> m_refs;
};

class ShaderCache
{
public:
  /// Acquires a shader program for a given hash, the dirty flag describes whether it's been built yet
  static CachedShader* acquireProgram(size_t hash, const Shader* who) {
    auto it = sm_programs.find(hash);
    if (it == sm_programs.end()) {
      sm_programs.insert(std::make_pair(hash, CachedShaderInternal(glCreateProgram(), hash)));
      it = sm_programs.find(hash);
    }
    it->second.m_refs.insert(who);
    return &it->second;
  }

  /// Releases a shader program so it can be deleted when nothing is using it anymore
  static void releaseProgram(size_t hash, const Shader* who) {
    auto it = sm_programs.find(hash);
    if (it != sm_programs.end()) {
      it->second.m_refs.erase(who);
      if (sm_deleteUnused && it->second.m_refs.empty()) {
        glDeleteProgram(it->second.m_prog);
        sm_programs.erase(it);
      }
    }
  }

  static void resetProgram(CachedShader* shader) {
    glDeleteProgram(shader->m_prog);
    shader->m_prog = glCreateProgram();
  }

  /// Whether to delete shader cores when they're no longer in use
  static const bool sm_deleteUnused = false;

private:
  static std::map<size_t, CachedShaderInternal> sm_programs;
};

std::map<size_t, CachedShaderInternal> ShaderCache::sm_programs;

Shader::~Shader()
{
  FileWatchService::Instance().removeWatch(this);
  if (m_cachedShader.has_value())
    ShaderCache::releaseProgram(m_cachedShader.value()->m_hash, this);
}

void Shader::load(const char* srcPath)
{
  m_filepath = srcPath;
  m_filename = filenameOf(srcPath);
  m_dirty = true;
}

void Shader::loadFromFile(const char* srcPath)
{
  m_filepath = srcPath;
  m_filename = filenameOf(srcPath);

  std::ifstream t(srcPath);
  std::stringstream buffer;
  buffer << genHeader();
  buffer << t.rdbuf();

  std::string shaderSource = buffer.str();
  auto shaderHash = std::hash<std::string>()(shaderSource);

  printf("Loading shader %s (%zx)\n", m_filename.c_str(), shaderHash);

  if (m_cachedShader.has_value())
    ShaderCache::releaseProgram(m_cachedShader.value()->m_hash, this);
  m_cachedShader = ShaderCache::acquireProgram(shaderHash, this);

  if (m_cachedShader.value()->m_dirty) {
    m_cachedShader.value()->m_dirty = false;
    ShaderCache::resetProgram(m_cachedShader.value());
    build(shaderSource);
  }

  // Check the attrib/uniform locations
  for (auto& attrib : m_attribs) {
    int loc = glGetAttribLocation(m_cachedShader.value()->m_prog, attrib.first.c_str());
    attrib.second.unused = (loc == -1);
#if LOG_UNUSED_SYMBOLS
    if (attrib.second.unused) {
      printf("Attribute is unused: %s\n", attrib.first.c_str());
    }
#endif
  }
  for (auto& uniform : m_uniforms) {
    int loc = glGetUniformLocation(m_cachedShader.value()->m_prog, uniform.first.c_str());
    uniform.second.unused = (loc == -1);
#if LOG_UNUSED_SYMBOLS
    if (uniform.second.unused) {
      printf("Uniform is unused: %s\n", uniform.first.c_str());
    }
#endif
  }

  FileWatchService::Instance().removeWatch(this);
  std::string shaderDir = dirnameOf(srcPath);
  FileWatchService::Instance().addWatch(shaderDir.c_str(), this);
}

void Shader::build(const std::string& shaderSource)
{
  printf("Building shader\n");

  auto prog = m_cachedShader.value()->m_prog;

  GLint vert = glCreateShader(GL_VERTEX_SHADER);
  GLint frag = glCreateShader(GL_FRAGMENT_SHADER);

  // Set back if there's a failure building
  m_cachedShader.value()->m_valid = true;

  try {
    std::stringstream vertShaderSS;
    vertShaderSS << "#version 330" << std::endl;
    vertShaderSS << "#define BUILDING_VERTEX_SHADER" << std::endl;
    vertShaderSS << shaderSource;
    std::string vertShader = vertShaderSS.str();

    const char* pVertShader[] = { vertShader.c_str() };
    glShaderSource(vert, 1, pVertShader, nullptr);
    glCompileShader(vert);
    GLint vertCompiled;
    glGetShaderiv(vert, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != GL_TRUE) {
      GLsizei logLen = 0;
      GLchar msg[1024];
      glGetShaderInfoLog(vert, 1023, &logLen, msg);
      fprintf(stderr, "Failed to compile vertex shader: \n%s\n", msg);
      openInBrowser(genShaderErrorPage(vertShader, msg, "Vertex"));
      throw std::runtime_error("");
    }

    std::stringstream fragShaderSS;
    fragShaderSS << "#version 330" << std::endl;
    fragShaderSS << "#define BUILDING_FRAGMENT_SHADER" << std::endl;
    fragShaderSS << shaderSource;
    std::string fragShader = fragShaderSS.str();

    const char* pFragShader[] = { fragShader.c_str() };
    glShaderSource(frag, 1, pFragShader, nullptr);
    glCompileShader(frag);
    GLint fragCompiled;
    glGetShaderiv(frag, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != GL_TRUE) {
      GLsizei logLen = 0;
      GLchar msg[1024];
      glGetShaderInfoLog(frag, 1023, &logLen, msg);
      fprintf(stderr, "Failed to compile fragment shader: \n%s\n", msg);
      openInBrowser(genShaderErrorPage(fragShader, msg, "Fragment"));
      throw std::runtime_error("");
    }

    glAttachShader(m_cachedShader.value()->m_prog, vert);
    glAttachShader(m_cachedShader.value()->m_prog, frag);
    glLinkProgram(m_cachedShader.value()->m_prog);

    GLint progLinked;
    glGetProgramiv(m_cachedShader.value()->m_prog, GL_LINK_STATUS, &progLinked);
    if (progLinked != GL_TRUE) {
      GLsizei logLen = 0;
      GLchar msg[1024];
      glGetProgramInfoLog(m_cachedShader.value()->m_prog, 1024, &logLen, msg);
      fprintf(stderr, "Failed to link program: \n%s\n", msg);
      throw std::runtime_error("");
    }
  }
  catch(...) {
    m_cachedShader.value()->m_valid = false;
  }

  // Clean up shaders
  glDeleteShader(vert);
  glDeleteShader(frag);
}

std::string Shader::genHeader()
{
  std::stringstream ss;
  ss << "#extension GL_ARB_explicit_uniform_location : enable" << std::endl;
  ss << "#extension GL_ARB_separate_shader_objects : enable" << std::endl;

  // Attributes
  ss << "#ifdef BUILDING_VERTEX_SHADER" << std::endl;
  for (auto& attr : m_attribs) {
    ss << "#define ATTR_" << attr.first << std::endl;
    ss << "layout(location=" << std::to_string(attr.second.location).c_str() << ") in ";
    switch (attr.second.type) {
      case AttribType::Float: ss << "float "; break;
      case AttribType::Vec2: ss << "vec2 "; break;
      case AttribType::Vec3: ss << "vec3 "; break;
      case AttribType::Vec4: ss << "vec4 "; break;
      default: ss << "unknown "; break;
    }
    ss << attr.first.c_str() << ";" << std::endl;
  }
  ss << "#endif" << std::endl;

  // Uniforms
  for (auto& uniform : m_uniforms) {
    if (uniform.second.enabled)
      ss << "#define UNI_" << uniform.first << std::endl;
    ss << "layout(location=" << std::to_string(uniform.second.location).c_str() << ") uniform ";
    switch (uniform.second.type) {
      case UniformType::Float: ss << "float "; break;
      case UniformType::Mat4: ss << "mat4 "; break;
      case UniformType::Sampler2D: ss << "sampler2D "; break;
      case UniformType::Sampler3D: ss << "sampler3D "; break;
      default: ss << "unknown "; break;
    }
    ss << uniform.first.c_str() << ";" << std::endl;
  }

  return ss.str();
}

void Shader::bind()
{
  if ((m_dirty || m_cachedShader.value()->m_dirty) && !m_filepath.empty()) {
    m_dirty = false;
    loadFromFile(m_filepath.c_str());
  }

  glUseProgram(m_cachedShader.value()->m_prog);
}

void Shader::unbind()
{
  glUseProgram(0);
}

void Shader::addAttrib(const char* name, AttribType type)
{
  m_attribs[name] = Attribute{m_nextAttribLocation++, type, true};
}

void Shader::bindAttrib(const char* name, int size, AttribType type, size_t stride, int offset)
{
  auto it = m_attribs.find(name);
  if (it != m_attribs.end() && !it->second.unused) {
    assert(it->second.location != -1);
    GLenum glType = attribGlType(type);
    glEnableVertexAttribArray(it->second.location);
    glVertexAttribPointer(it->second.location, size, attribGlType(type), GL_FALSE, static_cast<GLsizei>(stride), reinterpret_cast<void*>(static_cast<uintptr_t>(offset)));
  }
  else if (it == m_attribs.end()) {
    fprintf(stderr, "Attempt to bind nonexistent attrib: %s\n", name);
  }
}

void Shader::unbindAttrib(const char* name)
{
  auto it = m_attribs.find(name);
  if (it != m_attribs.end() && !it->second.unused) {
    glDisableVertexAttribArray(it->second.location);
  }
}

void Shader::addUniform(const char* name, UniformType type)
{
  m_uniforms[name] = Uniform{m_nextUniformLocation++, type, true, true};
}

void Shader::setUniform(const char* name, UniformType type, const void* buf)
{
  auto it = m_uniforms.find(name);
  if (it != m_uniforms.end() && !it->second.unused) {
    switch (type) {
      case UniformType::Float: glUniform1fv(it->second.location, 1, static_cast<const float*>(buf)); break;
      case UniformType::Mat4: glUniformMatrix4fv(it->second.location, 1, GL_FALSE, static_cast<const float*>(buf)); break;
      case UniformType::Sampler2D:
      case UniformType::Sampler3D: {
        TextureUnit ts = *static_cast<const TextureUnit*>(buf);
        glUniform1i(it->second.location, static_cast<int>(ts));
        break;
      }
      default: assert(false); break;
    }
  }
  else if (it == m_uniforms.end()) {
    fprintf(stderr, "Attempt to bind nonexistent uniform: %s\n", name);
  }
}

void Shader::setUniformEnabled(const char* name, bool enabled)
{
  auto it = m_uniforms.find(name);
  if (it != m_uniforms.end()) {
    if (it->second.enabled != enabled) {
      it->second.enabled = enabled;
      m_dirty = true;
    }
  }
}

void Shader::handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action)
{
  if (filename == m_filename) {
    m_dirty = true;
    m_cachedShader.value()->m_dirty = true;
  }
}

}
}