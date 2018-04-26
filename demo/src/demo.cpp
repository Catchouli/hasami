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
  
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, "res/buddha.obj");

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

  // Run through and check if it's indexed
  bool indexed = false;
  /*for (const auto& shape : shapes) {
    for (const auto& idx : shape.mesh.indices) {
      if (indexed && (idx.normal_index != -1 && idx.normal_index != idx.vertex_index) || (idx.texcoord_index != -1 && idx.texcoord_index != idx.vertex_index)) {
        indexed = false;
        break;
      }
    }
    if (!indexed)
      break;
  }*/

  printf("Model indexed: %s\n", indexed ? "true" : "false");

  struct Vertex { glm::vec3 pos; glm::vec3 nrm; glm::vec2 texCoord; };
  std::vector<Vertex> vertexBuf;
  std::vector<int> indexBuf;

  if (indexed) {
    throw;
  }
  else {
    size_t offset = 0;
    for (const auto& shape : shapes) {
      for (const auto& verts : shape.mesh.num_face_vertices) {
        if (verts != 3) {
          printf("Polygon with more or less than 3 sides detected in obj model, skipping\n");
          offset += verts;
          continue;
        }

        for (int i = 0; i < verts; ++i) {
          const float zero[] = { 0.0f, 0.0f, 0.0f, 0.0f };

          const auto& idx = shape.mesh.indices[offset];

          const auto& v = idx.vertex_index != -1 ? (float*)&attrib.vertices[idx.vertex_index*3] : zero;
          const auto& n = idx.normal_index != -1 ? (float*)&attrib.normals[idx.normal_index*3] : zero;
          const auto& u = idx.texcoord_index != -1 ? (float*)&attrib.texcoords[idx.texcoord_index*3] : zero;

          vertexBuf.push_back({ glm::vec3(v[0], v[1], v[2]), glm::vec3(n[0], n[1], n[2]), glm::vec2(u[0], u[1]) });

          offset++;
        }
      }
    }
  }

  // loop through and generate normals
  std::vector<std::vector<glm::vec3>> m_vertexNormals;
  m_vertexNormals.resize(vertexBuf.size());
  for (int i = 0; i < vertexBuf.size() / 3; ++i) {
    auto& a = vertexBuf[i*3+0];
    auto& b = vertexBuf[i*3+1];
    auto& c = vertexBuf[i*3+2];

    glm::vec3 nrm = -glm::normalize(glm::cross(b.pos - a.pos, c.pos - a.pos));
    a.nrm = b.nrm = c.nrm = nrm;
  }

  GLuint buf;
  glGenBuffers(1, &buf);
  glBindBuffer(GL_ARRAY_BUFFER, buf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexBuf.size(), vertexBuf.data(), GL_STATIC_DRAW);

  bool running = true;
  while (running) {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT)
        running = false;
    }

    static float rotation = 0.0f;
    rotation += 0.01f;

    glm::mat4 proj = glm::perspective(3.141f / 2.0f, 1.0f, 0.1f, 50.0f);
    glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -1.0f));
    glm::mat4 rot = glm::rotate(glm::mat4(), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 pivot = glm::translate(glm::mat4(), glm::vec3(-0.5f, -0.5f, -0.5f));
    pivot = glm::mat4();
    glm::mat4 obj = glm::rotate(glm::mat4(), -3.141f/2.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 mv = trans * rot * pivot * obj;
    glm::mat4 mvp = proj * mv;

    glUseProgram(prog);

    auto loc = glGetUniformLocation(prog, "mv");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mv[0][0]);

    loc = glGetUniformLocation(prog, "mvp");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]);

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(prog);

    glEnable(GL_DEPTH_TEST);

    auto pos = glGetAttribLocation(prog, "pos");
    auto nrm = glGetAttribLocation(prog, "nrm");

    glBindBuffer(GL_ARRAY_BUFFER, buf);

    glEnableVertexAttribArray(pos);
    glEnableVertexAttribArray(nrm);
    glVertexAttribPointer(pos, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(0));
    glVertexAttribPointer(nrm, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(3*sizeof(float)));

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertexBuf.size());

    glDisableVertexAttribArray(pos);
    glDisableVertexAttribArray(nrm);

    SDL_GL_SwapWindow(win);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);

  return 0;
}