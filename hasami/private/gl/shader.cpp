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
  GLuint indices[bufSize];
  GLint offset[bufSize];

  glGetProgramiv(m_prog, GL_ACTIVE_UNIFORM_BLOCKS, &count);
  
  m_uniformBlocks.clear();
  for (i = 0; i < count; i++) {
    glGetActiveUniformBlockName(m_prog, i, bufSize, &length, name);
    glGetUniformIndices(m_prog, 4, names, indices);
    m_uniformBlocks[name] = i;
    //glGetActiveUniformBlockiv(m_prog, i, )
  }

  int j = 9;
}

}
}