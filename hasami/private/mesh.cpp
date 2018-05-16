#define _CRT_SECURE_NO_WARNINGS

#include "mesh.hpp"
#include "shader.hpp"

#include "glm.hpp"
#include "tiny_obj_loader.h"

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
    default: return 0;
  }
}

Mesh::Mesh()
{

}

void Mesh::draw(const Shader& shader, const glm::mat4& projection, const glm::mat4& modelview)
{
  glm::mat4 mvp = projection * modelview;

  // Bind shader
  auto prog = shader.prog();
  glUseProgram(prog);

  // Uniforms
  auto loc = glGetUniformLocation(prog, "mv");
  glUniformMatrix4fv(loc, 1, GL_FALSE, &modelview[0][0]);

  loc = glGetUniformLocation(prog, "mvp");
  glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]);

  // Enable attributes
  std::vector<GLint> locations;
  size_t lastOffset = 0;
  for (int i = 0; i < (int)m_attrib.size(); ++i) {
    const auto& attr = m_attrib[i];
    const auto loc = glGetAttribLocation(prog, m_attrib[i].name.c_str());
    locations.push_back(loc);

    size_t offset = (attr.offset.has_value() ? attr.offset.value() : lastOffset);
    lastOffset = offset + attr.size * glSize(attr.type);

    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, attr.size, attr.type, false, m_buf.stride(), (void*)offset);
  }

  // Draw buffer
  m_buf.bind(GL_ARRAY_BUFFER);
  glDrawArrays(GL_TRIANGLES, 0, m_buf.size());

  // Disable attributes
  for (const auto& loc : locations) {
    glDisableVertexAttribArray(loc);
  }
}

void Mesh::loadObj(const char* path)
{
  std::string cache = std::string(path) + ".cache";

  if (loadCachedObj(cache.c_str()))
    return;

  // Structures
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  // Load obj
  std::string err;
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path);
  if (!err.empty()) {
    fprintf(stderr, "%s: %s\n", (ret ? "warning" : "error"), err.c_str());
  }

  // Create vertex buffer
  std::vector<Vertex> vertexBuf;

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

  // Loop through and generate normals
  std::vector<std::vector<glm::vec3>> m_vertexNormals;
  m_vertexNormals.resize(vertexBuf.size());
  for (int i = 0; i < vertexBuf.size() / 3; ++i) {
    auto& a = vertexBuf[i*3+0];
    auto& b = vertexBuf[i*3+1];
    auto& c = vertexBuf[i*3+2];

    glm::vec3 nrm = -glm::normalize(glm::cross(b.pos - a.pos, c.pos - a.pos));
    a.nrm = b.nrm = c.nrm = nrm;
  }

  // Set buffers
  m_buf.set((float*)vertexBuf.data(), (GLsizei)vertexBuf.size()*(sizeof(Vertex)/sizeof(float)), sizeof(Vertex), GL_STATIC_DRAW);

  // Store attributes
  m_attrib.clear();
  m_attrib.push_back(Attrib("pos", 3, GL_FLOAT));
  m_attrib.push_back(Attrib("nrm", 3, GL_FLOAT));
  m_attrib.push_back(Attrib("uv", 2, GL_FLOAT));

  // Write cache out
  writeCachedObj(cache.c_str(), vertexBuf, m_attrib);
}

bool Mesh::loadCachedObj(const char* path)
{
  const int version = 1;

  FILE* fd = fopen(path, "rb");
  if (!fd) {
    return false;
  }

  int vers = -1;
  fread(&vers, sizeof(int), 1, fd);
  if (vers != version) {
    fclose(fd);
    return false;
  }

  int vbufSize = -1;
  fread(&vbufSize, sizeof(int), 1, fd);

  std::vector<Vertex> vbuf;
  for (int i = 0; i < vbufSize; ++i) {
    Vertex v;
    fread(&v, sizeof(Vertex), 1, fd);
    vbuf.push_back(v);
  }

  m_buf.set((float*)vbuf.data(), (GLsizei)vbuf.size()*(sizeof(Vertex)/sizeof(float)), sizeof(Vertex), GL_STATIC_DRAW);

  int attribSize = -1;
  fread(&attribSize, sizeof(int), 1, fd);

  for (int i = 0; i < attribSize; ++i) {
    int nameSize;
    fread(&nameSize, sizeof(int), 1, fd);
    char* buf = (char*)calloc(1, nameSize+1);
    fread(buf, 1, nameSize, fd);
    std::string name = buf;
    free(buf);

    bool hasOffset;
    fread(&hasOffset, sizeof(bool), 1, fd);

    int offset;
    fread(&offset, sizeof(int), 1, fd);

    GLint size;
    fread(&size, sizeof(GLint), 1, fd);

    GLenum type;
    fread(&type, sizeof(GLenum), 1, fd);

    m_attrib.push_back(Attrib(name, size, type, hasOffset ? offset : std::optional<int>()));
  }

  fclose(fd);
  return true;
}

void Mesh::writeCachedObj(const char* path, const std::vector<Vertex>& vbuf, const std::vector<Attrib>& attribs)
{
  const int version = 1;

  FILE* fd = fopen(path, "wb");
  if (!fd) {
    return;
  }

  fwrite(&version, sizeof(int), 1, fd);

  int vbufSize = (int)vbuf.size();
  fwrite(&vbufSize, sizeof(int), 1, fd);

  for (const auto& vert : vbuf) {
    fwrite(&vert, sizeof(Vertex), 1, fd);
    /*fwrite(&vert.pos[0], sizeof(float), 1, fd);
    fwrite(&vert.pos[1], sizeof(float), 1, fd);
    fwrite(&vert.pos[2], sizeof(float), 1, fd);
    fwrite(&vert.nrm[0], sizeof(float), 1, fd);
    fwrite(&vert.nrm[1], sizeof(float), 1, fd);
    fwrite(&vert.nrm[2], sizeof(float), 1, fd);
    fwrite(&vert.texCoord[0], sizeof(float), 1, fd);
    fwrite(&vert.texCoord[1], sizeof(float), 1, fd);*/
  }

  int attribSize = (int)attribs.size();
  fwrite(&attribSize, sizeof(int), 1, fd);

  for (const auto& attrib : attribs) {
    int nameSize = (int)attrib.name.size();
    fwrite(&nameSize, sizeof(int), 1, fd);
    fwrite(attrib.name.data(), nameSize, 1, fd);

    bool hasOffset = attrib.offset.has_value();
    fwrite(&hasOffset, sizeof(bool), 1, fd);

    int offset = hasOffset ? attrib.offset.value() : -1;
    fwrite(&offset, sizeof(int), 1, fd);

    fwrite(&attrib.size, sizeof(GLint), 1, fd);
    fwrite(&attrib.type, sizeof(GLenum), 1, fd);
  }

  fclose(fd);
}