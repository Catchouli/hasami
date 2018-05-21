#include "gl/glshader.hpp"

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <sstream>

void openInBrowser(std::string);
std::string genShaderErrorPage(const std::vector<const char*>& source, std::string errors);

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

Shader::Shader()
  : m_nextAttribLocation(0)
  , m_nextUniformLocation(0)
{
}

void Shader::load(const char* srcPath)
{
  std::ifstream t(srcPath);
  std::stringstream buffer;
  buffer << t.rdbuf();
  std::string shaderSource = buffer.str();

  std::string header = genHeader();

  std::vector<const char*> vertShader = {
    "#version 330\r\n",
    "#define BUILDING_VERTEX_SHADER\r\n",
    header.c_str(),
    shaderSource.c_str()
  };

  m_prog = glCreateProgram();

  GLint vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, static_cast<int>(vertShader.size()), vertShader.data(), nullptr);
  glCompileShader(vert);
  GLint vertCompiled;
  glGetShaderiv(vert, GL_COMPILE_STATUS, &vertCompiled);
  if (vertCompiled != GL_TRUE) {
    GLsizei logLen = 0;
    GLchar msg[1024];
    glGetShaderInfoLog(vert, 1023, &logLen, msg);
    fprintf(stderr, "Failed to compile vertex shader: \n%s\n", msg);
    openInBrowser(genShaderErrorPage(vertShader, msg));
    return;
  }

  std::vector<const char*> fragShader = {
    "#version 330\r\n",
    "#define BUILDING_FRAGMENT_SHADER\r\n",
    header.c_str(),
    shaderSource.c_str()
  };

  GLint frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, static_cast<int>(fragShader.size()), fragShader.data(), nullptr);
  glCompileShader(frag);
  GLint fragCompiled;
  glGetShaderiv(frag, GL_COMPILE_STATUS, &fragCompiled);
  if (fragCompiled != GL_TRUE) {
    GLsizei logLen = 0;
    GLchar msg[1024];
    glGetShaderInfoLog(frag, 1023, &logLen, msg);
    fprintf(stderr, "Failed to compile fragment shader: \n%s\n", msg);
    openInBrowser(genShaderErrorPage(fragShader, msg));
    return;
  }

  glAttachShader(m_prog, vert);
  glDeleteShader(vert);
  glAttachShader(m_prog, frag);
  glDeleteShader(frag);
  glLinkProgram(m_prog);

  GLint progLinked;
  glGetProgramiv(m_prog, GL_LINK_STATUS, &progLinked);
  if (progLinked != GL_TRUE) {
    GLsizei logLen = 0;
    GLchar msg[1024];
    glGetProgramInfoLog(m_prog, 1024, &logLen, msg);
    fprintf(stderr, "Failed to link program: \n%s\n", msg);
    return;
  }

  // Check the attrib/uniform locations
  for (auto& attrib : m_attribs) {
    int loc = glGetAttribLocation(m_prog, attrib.first.c_str());
    if (loc != attrib.second.location) {
      fprintf(stderr, "Attribute is unused: %s\n", attrib.first.c_str());
      attrib.second.location = -1;
    }
  }
  for (auto& uniform : m_uniforms) {
    int loc = glGetUniformLocation(m_prog, uniform.first.c_str());
    if (loc != uniform.second.location) {
      fprintf(stderr, "Uniform is unused: %s\n", uniform.first.c_str());
      uniform.second.location = -1;
    }
  }
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
      case AttribType::Vec3: ss << "vec3 "; break;
      case AttribType::Vec4: ss << "vec4 "; break;
      default: ss << "unknown "; break;
    }
    ss << attr.first.c_str() << ";" << std::endl;
  }
  ss << "#endif" << std::endl;

  // Uniforms
  for (auto& uniform : m_uniforms) {
    ss << "#define UNI_" << uniform.first << std::endl;
    ss << "layout(location=" << std::to_string(uniform.second.location).c_str() << ") uniform ";
    switch (uniform.second.type) {
      case UniformType::Float: ss << "float "; break;
      case UniformType::Mat4: ss << "mat4 "; break;
      default: ss << "unknown "; break;
    }
    ss << uniform.first.c_str() << ";" << std::endl;
  }

  return ss.str();
}

void Shader::bind()
{
  glUseProgram(m_prog);
}

void Shader::unbind()
{
  glUseProgram(0);
}

void Shader::addAttrib(const char* name, AttribType type)
{
  m_attribs[name] = Attribute{m_nextAttribLocation++, type};
}

void Shader::bindAttrib(const char* name, int size, AttribType type, size_t stride, int offset)
{
  auto it = m_attribs.find(name);
  if (it != m_attribs.end() && it->second.location != -1) {
    assert(it->second.location != -1);
    GLenum glType = attribGlType(type);
    glEnableVertexAttribArray(it->second.location);
    glVertexAttribPointer(it->second.location, size, attribGlType(type), GL_FALSE, static_cast<GLsizei>(stride), reinterpret_cast<void*>(static_cast<uintptr_t>(offset)));
  }
}

void Shader::unbindAttrib(const char* name)
{
  auto it = m_attribs.find(name);
  if (it != m_attribs.end() && it->second.location != -1) {
    glDisableVertexAttribArray(it->second.location);
  }
}

void Shader::addUniform(const char* name, UniformType type)
{
  m_uniforms[name] = Uniform{m_nextUniformLocation++, type};
}

void Shader::setUniform(const char* name, UniformType type, const void* buf)
{
  int loc = glGetUniformLocation(m_prog, name);

  auto it = m_uniforms.find(name);
  if (it != m_uniforms.end() && it->second.location != -1) {
    switch (type) {
      case UniformType::Float: glUniform1fv(it->second.location, 1, static_cast<const float*>(buf)); break;;
      case UniformType::Mat4: glUniformMatrix4fv(it->second.location, 1, GL_FALSE, static_cast<const float*>(buf)); break;;
      default: assert(false); break;;
    }
  }
}

}
}

// Error page stuff

#ifdef WIN32
#include <Shellapi.h>
#define mktemp _mktemp
#define _CRT_SECURE_NO_WARNINGS
void openInBrowser(std::string url)
{
  ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
#else
void openInBrowser(std::string) {}
#endif

#include <io.h>
#include <fstream>

std::string genShaderErrorPage(const std::vector<const char*>& source, std::string errors)
{
  char name[256];
#ifdef WIN32
  tmpnam_s(name, 255);
#else
  tmpnam(name);
#endif

  std::string filename = std::string(name) = ".html";

  std::ofstream file(filename);

  for (const char* block : source) {
    file << block;
  }

  return filename;
}