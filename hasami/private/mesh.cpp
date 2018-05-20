#define _CRT_SECURE_NO_WARNINGS

#include "mesh.hpp"

#include "glm.hpp"
#include "tiny_obj_loader.h"

namespace hs {

int typeSize(AttribType type) {
  switch (type) {
    case AttribType::Float: return 4;
    default: assert(false); return 0;
  }
}

Mesh::Mesh(Renderer& renderer)
{
  m_buf = std::shared_ptr<hs::Buffer>(renderer.createBuffer());
}

void Mesh::draw(Renderer& renderer, Shader& shader, const glm::mat4& projection, const glm::mat4& modelview)
{
  glm::mat4 mvp = projection * modelview;

  // Bind shader
  shader.bind();

  // Uniforms
  shader.setUniform("_mv", UniformType::Mat4, &modelview[0][0]);
  shader.setUniform("_mvp", UniformType::Mat4, &mvp[0][0]);

  // Enable attributes
  size_t lastOffset = 0;
  for (auto& attr : m_attrib) {
    size_t offset = (attr.offset.has_value() ? attr.offset.value() : lastOffset);
    lastOffset = offset + attr.size * typeSize(attr.type);

    shader.bindAttrib(attr.name.c_str(), static_cast<int>(attr.size), attr.type, m_buf->stride(), static_cast<int>(offset));
  }

  // Flush state
  renderer.stateManager()->flush();

  // Draw buffer
  m_buf->bind(Buffer::Target::VertexBuffer);
  renderer.drawArrays(PrimitiveType::Triangles, 0, static_cast<int>(m_buf->size()));

  // Disable attributes
  for (auto& attr : m_attrib) {
    shader.unbindAttrib(attr.name.c_str());
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

    glm::vec3 nrm = glm::normalize(glm::cross(b.pos - a.pos, c.pos - a.pos));
    a.nrm = b.nrm = c.nrm = nrm;
  }

  // Set buffers
  m_buf->set((float*)vertexBuf.data(), static_cast<int>(vertexBuf.size())*(sizeof(Vertex)/sizeof(float)), sizeof(Vertex), Buffer::Usage::StaticDraw);

  // Store attributes
  m_attrib.clear();
  m_attrib.push_back(Attrib("pos", 3, AttribType::Float));
  m_attrib.push_back(Attrib("nrm", 3, AttribType::Float));
  m_attrib.push_back(Attrib("uv", 2, AttribType::Float));

  // Write cache out
  writeCachedObj(cache.c_str(), vertexBuf, m_attrib);
}

bool Mesh::loadCachedObj(const char* path)
{
  const int version = 3;

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

  m_buf->set((float*)vbuf.data(), static_cast<int>(vbuf.size())*(sizeof(Vertex)/sizeof(float)), sizeof(Vertex), Buffer::Usage::StaticDraw);

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

    size_t size;
    fread(&size, sizeof(size_t), 1, fd);

    int type;
    fread(&type, sizeof(int), 1, fd);

    m_attrib.push_back(Attrib(name, size, static_cast<AttribType>(type), hasOffset ? offset : std::optional<int>()));
  }

  fclose(fd);
  return true;
}

void Mesh::writeCachedObj(const char* path, const std::vector<Vertex>& vbuf, const std::vector<Attrib>& attribs)
{
  const int version = 3;

  FILE* fd = fopen(path, "wb");
  if (!fd) {
    return;
  }

  fwrite(&version, sizeof(int), 1, fd);

  int vbufSize = (int)vbuf.size();
  fwrite(&vbufSize, sizeof(int), 1, fd);

  for (const auto& vert : vbuf) {
    fwrite(&vert, sizeof(Vertex), 1, fd);
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

    fwrite(&attrib.size, sizeof(size_t), 1, fd);

    int type = static_cast<int>(attrib.type);
    fwrite(&type, sizeof(int), 1, fd);
  }

  fclose(fd);
}

}