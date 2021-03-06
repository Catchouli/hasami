#define _CRT_SECURE_NO_WARNINGS

#include "glm.hpp"
#include "tiny_obj_loader.h"
#include "util/util.hpp"
#include "renderer/renderer.hpp"
#include "renderer/mesh.hpp"
#include <optional>

namespace hs {

int typeSize(AttribType type) {
  switch (type) {
    case AttribType::Float: return 4;
    default: assert(false); return 0;
  }
}

Mesh::Mesh(Renderer& renderer, bool indexed)
  : m_indexed(indexed)
  , m_start(0)
  , m_count(0)
  , m_indexFormat(IndexFormat::U16)
{
  m_buf = std::shared_ptr<hs::Buffer>(renderer.createBuffer());
  if (indexed)
    m_ibuf = std::shared_ptr<hs::Buffer>(renderer.createBuffer());
}

void Mesh::draw(Renderer& renderer, hs::StandardMaterial& mat, const Context& ctx)
{
  auto& shader = *mat.shader();

  glm::mat4 mv = ctx.m_view * ctx.m_object;
  glm::mat4 mvp = ctx.m_projection * mv;

  // Set uniforms
  mat.model.set(ctx.m_object);
  mat.view.set(ctx.m_view);
  mat.projection.set(ctx.m_projection);
  mat.mvp.set(mvp);

  // Bind shader
  mat.flush();

  // Bind mesh buffer
  m_buf->bind(hs::BufferTarget::VertexBuffer);

  // Bind index buffer
  if (m_indexed)
    m_ibuf->bind(hs::BufferTarget::IndexBuffer);

  // Enable attributes
  size_t lastOffset = 0;
  for (auto& attr : m_attrib) {
    size_t offset = (attr.offset.has_value() ? attr.offset.value() : lastOffset);
    lastOffset = offset + attr.size * typeSize(attr.type);

    shader.bindAttrib(attr.name.c_str(), static_cast<int>(attr.size), attr.type, m_buf->stride(), static_cast<int>(offset));
  }

  // Flush state
  renderer.stateManager()->flush();

  // Draw mesh
  if (m_indexed)
    renderer.drawIndexed(PrimitiveType::Triangles, 0, static_cast<int>(m_buf->size()), m_indexFormat);
  else
    renderer.drawArrays(PrimitiveType::Triangles, 0, static_cast<int>(m_buf->size()));

  // Disable attributes
  for (auto& attr : m_attrib) {
    shader.unbindAttrib(attr.name.c_str());
  }
}

void Mesh::loadObj(const char* path, Normals normals)
{
  const int version = 1;

  size_t versionHash = 0;
  hs::hash_combine(versionHash, version, normals);

  std::string cache = std::string(path) + ".cache";

  if (loadCachedObj(cache.c_str(), versionHash))
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

  // Generate per-face normals
  std::vector<glm::vec3> faceNormals;
  std::map<size_t, std::vector<size_t>> vertexFaces;

  if (normals == Normals::Flat || normals == Normals::Smooth) {
    for (const auto& shape : shapes) {
      size_t offset = 0;
      size_t face = 0;
      for (const auto& verts : shape.mesh.num_face_vertices) {
        if (verts != 3) {
          printf("Polygon with more or less than 3 sides detected in obj model, skipping\n");
          offset += verts;
          face++;
          faceNormals.push_back(glm::vec3());
          continue;
        }

        int idx[3] = { shape.mesh.indices[offset+0].vertex_index
                        , shape.mesh.indices[offset+1].vertex_index
                        , shape.mesh.indices[offset+2].vertex_index };

        const auto& v0 = &attrib.vertices[3*idx[0]];
        const auto& v1 = &attrib.vertices[3*idx[1]];
        const auto& v2 = &attrib.vertices[3*idx[2]];

        glm::vec3 a = glm::vec3(v0[0], v0[1], v0[2]);
        glm::vec3 b = glm::vec3(v1[0], v1[1], v1[2]);
        glm::vec3 c = glm::vec3(v2[0], v2[1], v2[2]);

        glm::vec3 nrm = glm::normalize(glm::cross(b - a, c - a));
        faceNormals.push_back(nrm);
        vertexFaces[idx[0]].push_back(face);
        vertexFaces[idx[1]].push_back(face);
        vertexFaces[idx[2]].push_back(face);
      
        offset += verts;
        face++;
      }
    }
  }

  // Smooth normals
  std::vector<glm::vec3> smoothNormals;
  if (normals == Normals::Smooth) {
    const size_t vertCount = attrib.vertices.size() / 3;
    smoothNormals.resize(vertCount);
    for (int i = 0; i < vertCount; ++i) {
      const std::vector<size_t> faces = vertexFaces[i];
      float avgCoeff = 1.0f / faces.size();
      for (size_t face : faces) {
        smoothNormals[i] += avgCoeff * faceNormals[face];
      }
      smoothNormals[i] = glm::normalize(smoothNormals[i]);
    }
  }

  // Convert to non-indexed buffer (we need to concatenate the vertices together anyway)
  for (const auto& shape : shapes) {
    size_t offset = 0;
    size_t face = 0;
    for (const auto& verts : shape.mesh.num_face_vertices) {
      if (verts != 3) {
        printf("Polygon with more or less than 3 sides detected in obj model, skipping\n");
        offset += verts;
        face++;
        continue;
      }

      for (int i = 0; i < verts; ++i) {
        const float zero[] = { 0.0f, 0.0f, 0.0f, 0.0f };

        const auto& idx = shape.mesh.indices[offset];

        const float* v = idx.vertex_index != -1 ? &attrib.vertices[idx.vertex_index*3] : zero;
        const float* u = idx.texcoord_index != -1 ? &attrib.texcoords[idx.texcoord_index*2] : zero;
        const float* n = nullptr;

        if (idx.vertex_index < smoothNormals.size())
          n = (float*)&smoothNormals[idx.vertex_index];
        else if (face < faceNormals.size())
          n = (float*)&faceNormals[face];
        else
          n = idx.normal_index != -1 ? &attrib.normals[idx.normal_index*3] : zero;

        vertexBuf.push_back({ glm::vec3(v[0], v[1], v[2]), glm::vec3(n[0], n[1], n[2]), glm::vec2(u[0], u[1]) });

        offset++;
      }
      face++;
    }
  }

  // Set buffers
  m_buf->set((float*)vertexBuf.data(), static_cast<int>(vertexBuf.size())*(sizeof(Vertex)/sizeof(float)), sizeof(Vertex), BufferUsage::StaticDraw);

  // Store attributes
  m_attrib.clear();
  m_attrib.push_back(Attrib(Attrib_pos, 3, AttribType::Float));
  m_attrib.push_back(Attrib(Attrib_nrm, 3, AttribType::Float));
  m_attrib.push_back(Attrib(Attrib_tex0, 2, AttribType::Float));

  // Write cache out
  writeCachedObj(cache.c_str(), versionHash, vertexBuf, m_attrib);
}

bool Mesh::loadCachedObj(const char* path, size_t versionHash)
{
  FILE* fd = fopen(path, "rb");
  if (!fd) {
    return false;
  }

  size_t vers = -1;
  fread(&vers, sizeof(size_t), 1, fd);
  if (vers != versionHash) {
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

  m_buf->set((float*)vbuf.data(), static_cast<int>(vbuf.size())*(sizeof(Vertex)/sizeof(float)), sizeof(Vertex), BufferUsage::StaticDraw);

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

void Mesh::writeCachedObj(const char* path, size_t versionHash, const std::vector<Vertex>& vbuf, const std::vector<Attrib>& attribs)
{
  FILE* fd = fopen(path, "wb");
  if (!fd) {
    return;
  }

  fwrite(&versionHash, sizeof(size_t), 1, fd);

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
