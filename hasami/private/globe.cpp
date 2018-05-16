#include "globe.hpp"

void GlobeNode::draw(const Shader& shader, const glm::mat4& projection, const glm::mat4& modelview)
{
  generate(glm::vec3(0.0f, 0.0f, 3.0f));
  //ModelNode::draw(shader, projection, modelview);
}

void GlobeNode::generate(const glm::vec3& center)
{
  m_vert.clear();
  generateGlobe(center);

  if (!m_mesh)
    m_mesh = std::make_shared<Mesh>();

  m_mesh->m_attrib.clear();
  m_mesh->m_attrib.push_back(Attrib("pos", 3, GL_FLOAT));
  m_mesh->m_attrib.push_back(Attrib("nrm", 3, GL_FLOAT));
  m_mesh->m_attrib.push_back(Attrib("uvs", 2, GL_FLOAT));

  m_mesh->m_buf.set((float*)m_vert.data(), (GLsizei)m_vert.size()*(sizeof(Vertex)/sizeof(float)), sizeof(Vertex), GL_STATIC_DRAW);
}

void GlobeNode::subdivide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& center, float size)
{
  float ratio = 1.0f;
  float minsize = 0.005f;
  
  float third = 1.0f / 3.0f;
  float dot = glm::dot(third * a + third * b + third * c, glm::normalize(center));
  float dist = float(acos(glm::clamp(dot, -1.0f, 1.0f)) / glm::pi<float>());

  if (dist > 0.5) {
    return;
  }

  if (true || dist > ratio * size || size < minsize) {
    // Calc normal

    glm::vec3 nrm = -glm::normalize(glm::cross(b - a, c - a));
    m_vert.push_back({a, nrm, glm::vec2(0.0f)});
    m_vert.push_back({b, nrm, glm::vec2(0.0f)});
    m_vert.push_back({c, nrm, glm::vec2(0.0f)});
    return;
  }

  glm::vec3 p[6] = { a, b, c, (a + b) / 2.0f, (b + c) / 2.0f, (c + a) / 2.0f };
  int idx[12] = { 0, 3, 5, 5, 3, 4, 3, 1, 4, 5, 4, 2 };

  for (int i = 0; i < 4; ++i) {
    subdivide( glm::normalize(p[idx[i*3+0]])
             , glm::normalize(p[idx[i*3+1]])
             , glm::normalize(p[idx[i*3+2]])
             , center
             , size/2);
  }
}

void GlobeNode::generateGlobe(const glm::vec3& center)
{
  // create icosahedron
  float t = float(1.0 + sqrt(5.0)) / 2.0f;

  std::vector<glm::vec3> vb({
    { -1, t, 0 },{ 1, t, 0 },{ -1, -t, 0 },{ 1, -t, 0 },
    { 0, -1, t },{ 0, 1, t },{ 0, -1, -t },{ 0, 1, -t },
    { t, 0, -1 },{ t, 0, 1 },{ -t, 0, -1 },{ -t, 0, 1 },
    });
  std::vector<int> ib({
    0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
    1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 10, 7, 6, 7, 1, 8,
    3, 9, 4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9,
    4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7, 9, 8, 1
    });

    for (int i = 0; i < ib.size() / 3; ++i) {
      subdivide( glm::normalize(vb[ib[i*3]])
               , glm::normalize(vb[ib[i*3+1]])
               , glm::normalize(vb[ib[i*3+2]])
               , center
               , 1.0f);
    }
}