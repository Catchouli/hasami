#include "demo.hpp"
#include "globe.hpp"
#include "scenegraph.hpp"

#include "SDL.h"

#include <map>

Demo::Demo(hs::Window* window)
  : m_running(true)
{
  using namespace hs;

  m_camera = std::make_shared<FPSCamera>();
  m_camera->m_camSpeed = 0.01f;
  m_camera->m_camSensitivity = 0.001f;
  m_camera->m_pos.z = 1.5f;

  m_shader = std::shared_ptr<hs::Shader>(window->renderer()->createShader());
  m_shader->addAttrib("pos", AttribType::Vec3);
  m_shader->addAttrib("nrm", AttribType::Vec3);
  m_shader->addUniform("_mv", UniformType::Mat4);
  m_shader->addUniform("_mvp", UniformType::Mat4);
  m_shader->load("res/basic.glsl");

  m_scenegraph = std::make_shared<AssemblyNode>();

  m_buddha = std::make_shared<AssemblyNode>();
  m_buddha->setParent(m_scenegraph);

  auto buddhaModel = std::make_shared<ModelNode>();
  buddhaModel->setParent(m_buddha);
  buddhaModel->m_mesh = std::make_shared<Mesh>(*window->renderer());
  buddhaModel->m_mesh->loadObj("res/miku.obj", Mesh::Normals::Smooth);
  m_buddhaMesh = buddhaModel->m_mesh;

  m_globe = std::make_shared<AssemblyNode>();
  m_globe->setParent(m_scenegraph);

  auto globeModel = std::make_shared<GlobeNode>();
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
  m_buddha->m_pos.y = -1.0f;
  m_buddha->m_scale = glm::vec3(0.02f, 0.02f, 0.02f);
  //m_buddha->m_scale = glm::vec3(0.02f + 0.02f*(float)sin(x * 0.001f));
  m_buddha->m_rot = glm::rotate(glm::quat(), -3.14159f * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
  m_buddha->m_rot *= glm::rotate(glm::quat(), x * 0.001f, glm::vec3(0.0f, 1.0f, 0.0f));
  m_buddha->dirtyLocal();

  // Start render
  window->renderer()->clear(glm::vec4(0.0f), true, true);

  const auto& stateManager = window->renderer()->stateManager();

  stateManager->pushState(hs::DepthTest(true));
  stateManager->pushState(hs::CullFace(false));
  stateManager->pushState(hs::ClearColor(glm::vec4(0.0f)));
  stateManager->pushState(hs::PolygonMode(hs::RenderState::PolygonMode::Fill));

  if (m_shader)
    m_scenegraph->draw(*window->renderer(), *m_shader, m_camera->projMat(), m_camera->viewMat());

  stateManager->popState(hs::DepthTest());
  stateManager->popState(hs::CullFace());
  stateManager->popState(hs::ClearColor());
  stateManager->popState(hs::PolygonMode());
}