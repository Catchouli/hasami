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
#include "globe.hpp"

#include <set>
#include <memory>
#include <math.h>

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