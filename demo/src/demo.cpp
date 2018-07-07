#include "demo.hpp"
#include "globe.hpp"
#include "scenegraph.hpp"

#include "SDL.h"

#include <map>

Demo::Demo(hs::Window* window)
  : m_running(true)
{
  using namespace hs;

  // Create camera
  m_camera = std::make_shared<FPSCamera>();
  m_camera->m_camSpeed = 0.01f;
  m_camera->m_camSensitivity = 0.001f;
  m_camera->m_pos.z = 1.5f;

  // Create skybox
  auto skyboxMat = std::make_shared<hs::StandardMaterial>(window->renderer(), "res/skybox.glsl");

  // Create root node of scenegraph
  m_scenegraph = std::make_shared<AssemblyNode>();

  // Create main model
  m_buddha = std::make_shared<AssemblyNode>();
  m_buddha->setParent(m_scenegraph);

  m_buddhaModel = std::make_shared<ModelNode>();
  m_buddhaModel->setParent(m_buddha);
  m_buddhaModel->m_mesh = std::make_shared<Mesh>(*window->renderer());
  m_buddhaModel->m_mesh->loadObj("res/miku.obj", Mesh::Normals::Smooth);
  m_buddhaModel->m_mat = std::make_shared<hs::StandardMaterial>(window->renderer(), "res/basic.glsl");

  // Create the orbit and orbiting model
  m_orbitPivot = std::make_shared<AssemblyNode>();
  m_orbitPivot->setParent(m_scenegraph);

  m_orbitMiku = std::make_shared<AssemblyNode>();
  m_orbitMiku->setParent(m_orbitPivot);

  auto orbitMikuModel = std::make_shared<ModelNode>();
  orbitMikuModel->setParent(m_orbitMiku);
  orbitMikuModel->m_mesh = m_buddhaModel->m_mesh;
  orbitMikuModel->m_mat = m_buddhaModel->m_mat;

  // Create globe
  m_globe = std::make_shared<AssemblyNode>();
  m_globe->setParent(m_scenegraph);

  auto globeModel = std::make_shared<GlobeNode>();
  globeModel->setParent(m_globe);

  m_globe->m_enabled = false;

  // Load miku texture (for use in render())
  m_tex = std::shared_ptr<hs::Texture>(window->renderer()->createTexture());
  m_tex->load("res/miku.png");
}

void Demo::input(const SDL_Event* evt)
{
  if (evt->type == SDL_MOUSEMOTION) {
    m_camera->mouseMove(evt->motion.xrel, evt->motion.yrel);
  }
  if (evt->type == SDL_KEYDOWN && evt->key.keysym.scancode == SDL_SCANCODE_F1) {
    m_camera->m_lockCamera = !m_camera->m_lockCamera;
  }
}

void Demo::render(hs::Window* window)
{
  const float time = SDL_GetTicks() / 1000.0f;

  // Cycle texture on/off every couple of seconds
  bool even = (fmod(time, 2.0f) < 0.99f);
  m_buddhaModel->m_mat->albedo.set(even ? m_tex : nullptr);

  // The orbiting miku
  //m_orbitPivot->m_enabled = false;
  m_orbitPivot->m_rot = glm::rotate(glm::quat(), -time * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
  m_orbitPivot->dirtyLocal();
  m_orbitMiku->m_pos.x = -1.5f;
  m_orbitMiku->m_pos.y = -0.5f;
  m_orbitMiku->m_rot = glm::rotate(glm::quat(), -3.14159f * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
  m_orbitMiku->m_scale = glm::vec3(0.02f, 0.02f, 0.02f);
  m_orbitMiku->dirtyLocal();

  // Rotate buddha
  m_buddha->m_rot = glm::rotate(glm::quat(), -3.14159f * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
  //m_buddha->m_rot *= glm::rotate(glm::quat(), time, glm::vec3(0.0f, 1.0f, 0.0f));
  m_buddha->dirtyLocal();

  // Update camera
  auto* keyStates = SDL_GetKeyboardState(nullptr);
  m_camera->update(0.016f, keyStates);

  // Start render
  window->renderer()->clear(glm::vec4(0.0f), true, true);

  // Push states
  const auto& stateManager = window->renderer()->stateManager();
  stateManager->pushState(hs::DepthTest(true));
  stateManager->pushState(hs::CullFace(false));
  stateManager->pushState(hs::ClearColor(glm::vec4(0.0f)));
  stateManager->pushState(hs::PolygonMode(hs::RenderState::PolygonMode::Fill));

  // Construct context
  hs::Context ctx;
  ctx.m_time = time;
  ctx.m_projection = m_camera->projMat();
  ctx.m_view = m_camera->viewMat();
  ctx.m_object = glm::mat4();

  // Draw scenegraph
  m_scenegraph->draw(*window->renderer(), ctx);

  // Pop states
  stateManager->popState(hs::DepthTest());
  stateManager->popState(hs::CullFace());
  stateManager->popState(hs::ClearColor());
  stateManager->popState(hs::PolygonMode());
}