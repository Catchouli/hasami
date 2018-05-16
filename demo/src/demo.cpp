#include "demo.hpp"

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

class SceneNode
  : public std::enable_shared_from_this<SceneNode>
{
public:
  SceneNode();
  ~SceneNode();

  void setParent(std::shared_ptr<SceneNode> newParent);
  std::shared_ptr<SceneNode> parent() { return m_parent; }

  const std::set<std::shared_ptr<SceneNode>>& children() { return m_children; }

  virtual void draw(const Shader& shader, const glm::mat4& projection, const glm::mat4& view) = 0;

  std::shared_ptr<SceneNode> ptr() { return shared_from_this(); }

private:
  void addChild(std::shared_ptr<SceneNode> node);
  void removeChild(std::shared_ptr<SceneNode> node);

  std::shared_ptr<SceneNode> m_parent;
  std::set<std::shared_ptr<SceneNode>> m_children;
};

class AssemblyNode
  : public SceneNode
{
public:
  AssemblyNode();

  virtual void draw(const Shader& shader, const glm::mat4& projection, const glm::mat4& view) override;

  void dirtyLocal() { m_localDirty = true; }

  glm::vec3 m_pos;
  glm::quat m_rot;
  glm::vec3 m_scale;

private:
  void updateTransform();

  bool m_localDirty;
  glm::mat4 m_local;
};

class ModelNode
  : public SceneNode
{
public:
  virtual void draw(const Shader& shader, const glm::mat4& projection, const glm::mat4& view) override;

  std::shared_ptr<Mesh> m_mesh;
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
}

void renderTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
  glBegin(GL_TRIANGLES);
  glVertex3f(a.x, a.y, a.z);
  glVertex3f(b.x, b.y, b.z);
  glVertex3f(c.x, c.y, c.z);
  glEnd();
}


void subdivide(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& center, float size = 1.0f, float lod = 0.0f)
{
  float ratio = 1.0f;
  float minsize = 0.005f;
  
  float third = 1.0f / 3.0f;
  float dot = glm::dot(third * a + third * b + third * c, glm::normalize(center));
  float dist = float(acos(glm::clamp(dot, -1.0f, 1.0f)) / glm::pi<float>());

  dist = glm::mix(dist, 0.5f, lod);

  if (dist > 0.5) {
    return;
  }

  if (true || dist > ratio * size || size < minsize) {
    renderTriangle(a, b, c);
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

void renderGlobe(const glm::vec3& center, float lod)
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
      //renderTriangle(vb[ib[i*3]], vb[ib[i*3+1]], vb[ib[i*3+2]]);
      subdivide( glm::normalize(vb[ib[i*3]])
               , glm::normalize(vb[ib[i*3+1]])
               , glm::normalize(vb[ib[i*3+2]])
               , center
               , 1.0f
               , lod);
    }
}

int camX = 0, camY = 0;

void Demo::input(const SDL_Event* evt)
{
  if (evt->type == SDL_MOUSEMOTION) {
    camY += evt->motion.xrel;
    camX += evt->motion.yrel;
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

  glPolygonMode(GL_FRONT, GL_LINE);

  glm::mat4 mvp = m_camera->projMat() * m_camera->viewMat() * m;

  glLoadMatrixf(&mvp[0][0]);

  //glm::vec3 center = (glm::inverse(m) * glm::vec4(camPos, 1.0f));
  glm::vec3 center = glm::vec4(m_camera->m_pos, 1.0f);

  renderGlobe(center, 1.0f);

  m_scenegraph->draw(m_shader.value(), m_camera->projMat(), m_camera->viewMat());
}