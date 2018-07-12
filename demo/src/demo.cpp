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
  m_camera->m_pos.y = 1.0f;
  m_camera->m_pos.z = 3.5f;

  // Create skybox
  auto skyboxTex = std::shared_ptr<hs::Texture>(window->renderer()->createTexture());
  skyboxTex->load("res/sky.jpg");
  auto skyboxMat = std::make_shared<hs::StandardMaterial>(window->renderer(), "res/skybox.glsl");
  skyboxMat->albedo.set(skyboxTex);

  // Create root node of scenegraph and setup state
  m_scenegraph = std::make_shared<AssemblyNode>();
  m_scenegraph->addState(hs::DepthTest(true));
  m_scenegraph->addState(hs::CullFace(false));
  m_scenegraph->addState(hs::ClearColor(glm::vec4(0.0f)));
  m_scenegraph->addState(hs::PolygonMode(hs::RenderState::PolygonMode::Fill));

  // Create skybox
  auto skybox = std::make_shared<AssemblyNode>();
  skybox->setParent(m_scenegraph);
  skybox->m_scale = glm::vec3(10.0f);
  skybox->dirtyLocal();
  skybox->addState(hs::CullFace(false));

  auto skyboxModel = std::make_shared<ModelNode>();
  skyboxModel->setParent(skybox);
  skyboxModel->m_mesh = std::make_shared<Mesh>(*window->renderer());
  skyboxModel->m_mesh->loadObj("res/sphere.obj", Mesh::Normals::Smooth);
  skyboxModel->m_mat = skyboxMat;

  auto mikuTex = std::shared_ptr<hs::Texture>(window->renderer()->createTexture());
  mikuTex->load("res/miku.png");

  // Create cornell box
  auto cornellBox = std::make_shared<AssemblyNode>();
  cornellBox->setParent(m_scenegraph);

  auto cornellBoxModel = std::make_shared<ModelNode>();
  cornellBoxModel->setParent(cornellBox);
  cornellBoxModel->m_mesh = std::make_shared<Mesh>(*window->renderer());
  cornellBoxModel->m_mesh->loadObj("res/CornellBox/CornellBox-Original.obj", Mesh::Normals::Smooth);
  cornellBoxModel->m_mat = std::make_shared<hs::StandardMaterial>(window->renderer(), "res/basic.glsl");

  // Create miku model
  m_miku = std::make_shared<AssemblyNode>();
  m_miku->setParent(m_scenegraph);

  m_mikuModel = std::make_shared<ModelNode>();
  m_mikuModel->setParent(m_miku);
  m_mikuModel->m_mesh = std::make_shared<Mesh>(*window->renderer());
  m_mikuModel->m_mesh->loadObj("res/miku.obj", Mesh::Normals::Smooth);
  m_mikuModel->m_mat = std::make_shared<hs::StandardMaterial>(window->renderer(), "res/basic.glsl");
  m_mikuModel->m_mat->albedo.set(mikuTex);

  // Create globe
  m_globe = std::make_shared<AssemblyNode>();
  m_globe->setParent(m_scenegraph);

  auto globeModel = std::make_shared<GlobeNode>();
  globeModel->setParent(m_globe);

  // Disable globe and miku by default
  //skybox->m_enabled = false;
  //m_miku->m_enabled = false;
  cornellBox->m_enabled = false;
  m_globe->m_enabled = false;
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

  // Cycles between true and false every couple seconds
  bool even = (fmod(time, 2.0f) < 0.99f);

  // Update miku
  if (m_miku->m_enabled) {
    m_miku->m_rot = glm::rotate(glm::quat(), -3.14159f * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
    m_miku->m_scale = glm::vec3(0.05f, 0.05f, 0.05f) * (1.0f+0.2f*sin(time));
    m_miku->dirtyLocal();
  }

  // Update camera
  auto* keyStates = SDL_GetKeyboardState(nullptr);
  m_camera->update(0.016f, keyStates);

  // Render scenegraph
  hs::RenderParams params;
  params.m_time = time;
  m_scenegraph->render(*window->renderer(), *m_camera, params);
}