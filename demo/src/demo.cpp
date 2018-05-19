#include "demo.hpp"
#include "globe.hpp"
#include "scenegraph.hpp"
#include "gl/shader.hpp"

#include "SDL.h"

#include <map>

Demo::Demo()
  : m_running(true)
{
  using namespace hs;

  m_camera = std::make_shared<FPSCamera>();
  m_camera->m_camSpeed = 0.01f;
  m_camera->m_camSensitivity = 0.001f;
  m_camera->m_pos.z = 3.0f;

  //m_shader = gl::Shader("res/basic.glsl");
  m_shader = std::make_shared<BasicShader>();

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
  //globeModel->setParent(m_globe);
}

void Demo::input(const SDL_Event* evt)
{
  if (evt->type == SDL_MOUSEMOTION) {
    m_camera->mouseMove(evt->motion.xrel, evt->motion.yrel);
  }
}

void Demo::render(hs::Window* window)
{
  static float x = 0.0f;
  x += SDL_GetTicks() / 1000.0f;

  auto* keyStates = SDL_GetKeyboardState(nullptr);

  // Update camera
  m_camera->update(0.016f, keyStates);

  // Rotate buddha
  float scale = 2.0f + float(sin(x*0.001f));
  m_buddha->m_rot = glm::rotate(glm::quat(), -3.14159f * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
  //m_buddha->m_rot = glm::rotate(m_buddha->m_rot, scale, glm::vec3(0.0f, 1.0f, 0.0f));
  m_buddha->dirtyLocal();

  // Start render
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  //glPolygonMode(GL_FRONT, GL_LINE);

  if (m_shader)
    m_scenegraph->draw(*m_shader, m_camera->projMat(), m_camera->viewMat());
}