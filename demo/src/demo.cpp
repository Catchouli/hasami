#include "renderer.hpp"

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "glad/glad.h"
#include <stdio.h>
#include "tiny_obj_loader.h"
#include <fstream>
#include <sstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

int main(int argc, char** argv) {
  SDL_Window* win;
  SDL_Renderer* renderer;
  SDL_GLContext context;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    fprintf(stderr, "SDL error: %s\n", SDL_GetError());
    return 1;
  }

  if (SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &win, &renderer) < 0) {
    fprintf(stderr, "SDL error: %s\n", SDL_GetError());
    return 1;
  }

  context = SDL_GL_CreateContext(win);

  if (!gladLoadGL()) {
    fprintf(stderr, "Glad (opengl) failed to initialise\n");
    return 1;
  }

  // load resources
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string err;
  
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, "res/teapot.obj");

  if (!err.empty()) {
    fprintf(stderr, "%s: %s\n", (ret ? "warning" : "error"), err.c_str());
  }

  // Load shader
  std::ifstream t("res/basic.glsl");
  std::stringstream buffer;
  buffer << t.rdbuf();
  std::string shaderSource = buffer.str();

  const char* vertShader[] = {
    "#version 330\r\n",
    "#define BUILDING_VERTEX_SHADER\r\n",
    shaderSource.c_str()
  };

  GLint prog = glCreateProgram();

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

  glAttachShader(prog, vert);
  glAttachShader(prog, frag);
  glLinkProgram(prog);

  GLint progLinked;
  glGetProgramiv(prog, GL_LINK_STATUS, &progLinked);
  if (progLinked != GL_TRUE) {
    GLsizei logLen = 0;
    GLchar msg[1024];
    glGetProgramInfoLog(prog, 1024, &logLen, msg);
    fprintf(stderr, "Failed to link program: %s\n", msg);
  }

  glm::mat4 m = glm::translate(glm::perspective(60.0f, 1.0f, 0.1f, 10.0f), glm::vec3(0.0f, 0.0f, -0.5f));
  glUseProgram(prog);
  auto loc = glGetUniformLocation(prog, "mvp");
  glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);

  struct Vertex { glm::vec3 pos; glm::vec3 nrm; };
  std::vector<Vertex> vertexBuf;
  for (const auto& shape : shapes) {
    for (const auto& idx : shape.mesh.indices) {
      const float zero[] = { 0.0f, 0.0f, 0.0f, 0.0f };
      const auto& v = (idx.vertex_index < 0 ? zero : &attrib.vertices[idx.vertex_index*3]);
      const auto& n = (idx.normal_index < 0 ? zero : &attrib.normals[idx.normal_index*3]);
      vertexBuf.push_back({ glm::vec3(v[0], v[1], v[2]), glm::vec3(n[0], n[1], n[2]) });
    }
  }

  // loop through and generate normals
  for (int i = 0; i < vertexBuf.size() / 3; ++i) {
    auto& a = vertexBuf[i*3+0];
    auto& b = vertexBuf[i*3+1];
    auto& c = vertexBuf[i*3+2];

    glm::vec3 nrm = glm::normalize(glm::cross(b.pos - a.pos, c.pos - a.pos));
    a.nrm = b.nrm = c.nrm = nrm;
  }

  bool running = true;
  while (running) {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT)
        running = false;
    }

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(prog);

    glEnable(GL_DEPTH_TEST);

    auto pos = glGetAttribLocation(prog, "pos");
    auto nrm = glGetAttribLocation(prog, "nrm");

    glEnableVertexAttribArray(pos);
    glEnableVertexAttribArray(nrm);
    glVertexAttribPointer(pos, 3 * sizeof(float), GL_FLOAT, false, 6 * sizeof(float), (void*)(vertexBuf.data()));
    glVertexAttribPointer(nrm, 3 * sizeof(float), GL_FLOAT, false, 6 * sizeof(float), (void*)(vertexBuf.data()+3*sizeof(float)));
    glDisableVertexAttribArray(nrm);
    glDisableVertexAttribArray(nrm);

    glBegin(GL_TRIANGLES);
    for (const auto& vert : vertexBuf) {
      glVertex3f(vert.pos.x, vert.pos.y, vert.pos.z);
      glNormal3f(vert.nrm.x, vert.nrm.y, vert.nrm.z);
    }
    glEnd();

    SDL_GL_SwapWindow(win);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);

  return 0;
}