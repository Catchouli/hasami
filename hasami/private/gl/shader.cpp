#include "gl/shader.hpp"

#include <stdio.h>
#include <fstream>
#include <sstream>

namespace hs {
namespace gl {

Shader::Shader(const char* srcPath)
{
  load(srcPath);
}

void Shader::load(const char* srcPath)
{
  // todo: are we leaking shaders?

  // Load shader
  std::ifstream t(srcPath);
  std::stringstream buffer;
  buffer << t.rdbuf();
  std::string shaderSource = buffer.str();

  const char* vertShader[] = {
    "#version 330\r\n",
    "#define BUILDING_VERTEX_SHADER\r\n",
    shaderSource.c_str()
  };

  m_prog = glCreateProgram();

  GLint vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 3, vertShader, nullptr);
  glCompileShader(vert);
  GLint vertCompiled;
  glGetShaderiv(vert, GL_COMPILE_STATUS, &vertCompiled);
  if (vertCompiled != GL_TRUE) {
    GLsizei logLen = 0;
    GLchar msg[1024];
    glGetShaderInfoLog(vert, 1023, &logLen, msg);
    fprintf(stderr, "Failed to compile vertex shader: %s\n", msg);
    return;
  }

  const char* fragShader[] = {
    "#version 330\r\n",
    "#define BUILDING_FRAGMENT_SHADER\r\n",
    shaderSource.c_str()
  };

  GLint frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 3, fragShader, nullptr);
  glCompileShader(frag);
  GLint fragCompiled;
  glGetShaderiv(frag, GL_COMPILE_STATUS, &fragCompiled);
  if (fragCompiled != GL_TRUE) {
    GLsizei logLen = 0;
    GLchar msg[1024];
    glGetShaderInfoLog(frag, 1023, &logLen, msg);
    fprintf(stderr, "Failed to compile fragment shader: %s\n", msg);
    return;
  }

  glAttachShader(m_prog, vert);
  glAttachShader(m_prog, frag);
  glLinkProgram(m_prog);

  GLint progLinked;
  glGetProgramiv(m_prog, GL_LINK_STATUS, &progLinked);
  if (progLinked != GL_TRUE) {
    GLsizei logLen = 0;
    GLchar msg[1024];
    glGetProgramInfoLog(m_prog, 1024, &logLen, msg);
    fprintf(stderr, "Failed to link program: %s\n", msg);
    return;
  }

  readUniforms();
}

void Shader::readUniforms()
{
  GLint i;
  GLint count;

  GLint size; // size of the variable
  GLenum type; // type of the variable (float, vec3 or mat4, etc)

  const GLsizei bufSize = 16; // maximum name length
  GLchar name[bufSize]; // variable name in GLSL
  GLsizei length; // name length

  // get attributes
  glGetProgramiv(m_prog, GL_ACTIVE_ATTRIBUTES, &count);

  m_attributes.clear();
  for (i = 0; i < count; i++)
  {
    glGetActiveAttrib(m_prog, (GLuint)i, bufSize, &length, &size, &type, name);
    GLint loc = glGetAttribLocation(m_prog, name);
    m_attributes[name] = loc;
  }

  // get uniforms
  glGetProgramiv(m_prog, GL_ACTIVE_UNIFORMS, &count);

  m_uniforms.clear();
  for (i = 0; i < count; i++)
  {
    glGetActiveUniform(m_prog, (GLuint)i, bufSize, &length, &size, &type, name);
    GLint loc = glGetUniformLocation(m_prog, name);

    if (loc != -1) {
      m_uniforms[name] = loc;
    }
  }

  // get uniform blocks
  glGetProgramiv(m_prog, GL_ACTIVE_UNIFORM_BLOCKS, &count);
  
  m_uniformBlocks.clear();
  for (i = 0; i < count; i++) {
    glGetActiveUniformBlockName(m_prog, i, bufSize, &length, name);
    m_uniformBlocks[name] = i;
  }
}

void Shader::bind()
{
  glUseProgram(m_prog);
}

void Shader::flush()
{
  bind();

  if (m_varsDirty) {
    m_varsDirty = false;

    for (auto& var : m_shaderVars) {
      if (var.second->dirty()) {
        const int loc = glGetUniformLocation(m_prog, var.first.c_str());
        const int size = var.second->size();

        switch (var.second->type()) {
          case ShaderVarBase::Type::Mat4: glUniformMatrix4fv(loc, 1, GL_FALSE, static_cast<float*>(var.second->buf())); break;
          case ShaderVarBase::Type::Unknown: assert(false); break;
        }
      }
    }
  }
}

}
}