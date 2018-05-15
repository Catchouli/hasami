#include "shader.hpp"

#include <stdio.h>
#include <fstream>
#include <sstream>

Shader::Shader(const char* srcPath) {
  load(srcPath);
}

void Shader::load(const char* srcPath) {
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
  }
}