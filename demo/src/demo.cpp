#include "demo.hpp"

#include "globe.hpp"

#include "glad/glad.h"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "gtx/quaternion.hpp"

#include <set>
#include <memory>
#include <math.h>

class GlobeNode
  : public ModelNode
{
public:
  virtual void draw(const Shader& shader, const glm::mat4& projection, const glm::mat4& view) override;

private:
  void generate(const glm::vec3& center);
  void subdivide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& center, float size = 1.0f);
  void generateGlobe(const glm::vec3& center);

  std::vector<Vertex> m_vert;
};

SceneNode::SceneNode()
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::setParent(std::shared_ptr<SceneNode> newParent)
{
  if (m_parent)
    m_parent->removeChild(ptr());
  m_parent = newParent;
  if (m_parent)
    m_parent->addChild(ptr());
}

void SceneNode::addChild(std::shared_ptr<SceneNode> node)
{
  if (m_children.find(node) == m_children.end()) {
    m_children.insert(node);
  }
}

void SceneNode::removeChild(std::shared_ptr<SceneNode> node)
{
  auto it = m_children.find(node);
  if (it != m_children.end()) {
    m_children.erase(it);
  }
}

AssemblyNode::AssemblyNode()
  : m_localDirty(false)
  , m_scale(1.0f, 1.0f, 1.0f)
{
}

void AssemblyNode::draw(const Shader& shader, const glm::mat4& projection, const glm::mat4& parentmv)
{
  updateTransform();
  glm::mat4 modelview = parentmv * m_local;

  for (auto child : children()) {
    child->draw(shader, projection, modelview);
  }
}

void AssemblyNode::updateTransform()
{
  if (!m_localDirty)
    return;

  m_localDirty = false;
  m_local = glm::translate(glm::scale(glm::toMat4(m_rot), m_scale), m_pos);
}

void ModelNode::draw(const Shader& shader, const glm::mat4& projection, const glm::mat4& modelview)
{
  m_mesh->draw(shader, projection, modelview);
}

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

class FPSCamera
{
public:
  FPSCamera();
  void mouseMove(int x, int y);
  void update(float timeDelta, const Uint8* keyStates);

  const glm::mat4& projMat() { return m_proj; }
  const glm::mat4& viewMat() { return m_view; }

  float m_camSpeed;
  float m_camSensitivity;

  glm::vec3 m_pos;
  glm::vec2 m_rot;

private:
  glm::mat4 m_proj;
  glm::mat4 m_view;

  glm::vec3 m_forward;
  glm::vec3 m_left;
  glm::vec3 m_up;
};

FPSCamera::FPSCamera()
  : m_forward(0.0, 0.0, -1.0)
  , m_left(-1.0, 0.0, 0.0)
  , m_up(0.0, 1.0, 0.0)
  , m_camSpeed(1.0f)
  , m_camSensitivity(1.0f)
{
}

void FPSCamera::mouseMove(int x, int y)
{
  m_rot.y += x;
  m_rot.x += y;
}

void FPSCamera::update(float timeDelta, const Uint8* keyStates)
{
  glm::mat4 camRotX = glm::rotate(glm::mat4(), m_rot.x * m_camSensitivity, glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 camRotY = glm::rotate(glm::mat4(), m_rot.y * m_camSensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 camRot = camRotX * camRotY;

  m_forward = glm::vec3(0.0f, 0.0f, -1.0f) * glm::mat3(camRot);
  m_left = glm::vec3(-1.0f, 0.0f, 0.0f) * glm::mat3(camRot);
  m_up = glm::vec3(0.0f, 1.0f, 0.0f) * glm::mat3(camRot);

  if (keyStates[SDL_SCANCODE_W]) {
    m_pos += m_forward * m_camSpeed;
  }
  if (keyStates[SDL_SCANCODE_S]) {
    m_pos -= m_forward * m_camSpeed;
  }
  if (keyStates[SDL_SCANCODE_A]) {
    m_pos += m_left * m_camSpeed;
  }
  if (keyStates[SDL_SCANCODE_D]) {
    m_pos -= m_left * m_camSpeed;
  }
  if (keyStates[SDL_SCANCODE_E]) {
    m_pos += m_up * m_camSpeed;
  }
  if (keyStates[SDL_SCANCODE_Q]) {
    m_pos -= m_up * m_camSpeed;
  }

  m_proj = glm::perspective(3.141f / 2.0f, 1.0f, 0.001f, 50.0f);
  m_view = camRot * glm::translate(glm::mat4(), -m_pos);
}

Demo::Demo()
  : m_running(true)
{
  m_camera = std::make_shared<FPSCamera>();
  m_camera->m_camSpeed = 0.01f;
  m_camera->m_camSensitivity = 0.001f;
  m_camera->m_pos.z = 3.0f;

  m_shader = Shader("res/basic.glsl");

  m_scenegraph = std::make_shared<AssemblyNode>();

  m_buddha = std::make_shared<AssemblyNode>();
  m_buddha->setParent(m_scenegraph);

  auto buddhaModel = std::make_shared<ModelNode>();
  buddhaModel->setParent(m_buddha);
  buddhaModel->m_mesh = std::make_shared<Mesh>();
  buddhaModel->m_mesh->loadObj("res/buddha.obj");

  m_globe = std::make_shared<AssemblyNode>();
  m_globe->setParent(m_scenegraph);

  auto globeModel = std::make_shared<GlobeNode>();
  globeModel->setParent(m_globe);
}

void Demo::input(const SDL_Event* evt)
{
  if (evt->type == SDL_MOUSEMOTION) {
    m_camera->mouseMove(evt->motion.xrel, evt->motion.yrel);
  }
}

void Demo::render(Window* window)
{
  static float x = 0.0f;
  x += SDL_GetTicks() / 1000.0f;

  auto* keyStates = SDL_GetKeyboardState(nullptr);

  // Update camera
  m_camera->update(0.016f, keyStates);

  // Rotate buddha
  float scale = 2.0f + float(sin(x*0.001f));
  m_buddha->m_rot = glm::rotate(glm::quat(), -3.14159f * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
  m_buddha->m_rot = glm::rotate(m_buddha->m_rot, scale, glm::vec3(0.0f, 1.0f, 0.0f));
  m_buddha->dirtyLocal();

  // Start render
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  //glPolygonMode(GL_FRONT, GL_LINE);

  m_scenegraph->draw(m_shader.value(), m_camera->projMat(), m_camera->viewMat());
}