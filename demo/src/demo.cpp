#include "demo.hpp"
#include "globe.hpp"
#include "scenegraph.hpp"

#include "SDL.h"

#include <map>

#include <gtc/matrix_transform.hpp>

#ifdef USE_IMGUI
#include "imgui.hpp"
#endif

Demo::Demo(hs::Window* window)
  : m_running(true)
  , m_wireframe(false)
{
  using namespace hs;

  // Create camera
  m_camera = std::make_shared<FPSCamera>();
  m_camera->m_camSpeed = 0.01f;
  m_camera->m_camSensitivity = 0.001f;
  m_camera->m_pos = glm::vec3(-1.26f, 0.7f, 1.37f);
  m_camera->m_rot = glm::vec2(359.0f, 787.0f);
  m_camera->update(0.0f, nullptr);
  m_camera->m_lockCamera = true; //^ lock unless we have the mouse button pressed

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
  m_scenegraph->addState(hs::PolygonMode(m_wireframe ? hs::RenderState::PolygonMode::Line : hs::RenderState::PolygonMode::Fill));

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
  globeModel->m_mat = std::make_shared<hs::StandardMaterial>(window->renderer(), "res/basic.glsl");

  // Disable globe and miku by default
  skybox->m_enabled = false;
  m_miku->m_enabled = false;
  cornellBox->m_enabled = false;
  //m_globe->m_enabled = false;
}

void Demo::input(const SDL_Event* evt)
{
  #ifdef USE_IMGUI
  auto& io = ImGui::GetIO();
  bool hasMouseFocus = !io.WantCaptureMouse;
  #else
  bool hasMouseFocus = true;
  #endif


  if (evt->type == SDL_MOUSEMOTION) {
    m_camera->mouseMove(evt->motion.xrel, evt->motion.yrel);
  }

  if (evt->type == SDL_KEYDOWN) {
    if((evt->key.keysym.mod & KMOD_CTRL) && evt->key.keysym.scancode == SDL_SCANCODE_W) {
      m_wireframe = !m_wireframe;
      m_scenegraph->addState(hs::PolygonMode(m_wireframe ? hs::RenderState::PolygonMode::Line : hs::RenderState::PolygonMode::Fill));
    }
  }

  if (evt->type == SDL_MOUSEBUTTONDOWN || evt->type == SDL_MOUSEBUTTONUP) {
    if (evt->button.button == SDL_BUTTON_LEFT) {
      m_camera->m_lockCamera = !hasMouseFocus || (evt->button.state != SDL_PRESSED);
    }
  }
}

void Demo::render(hs::Window* window)
{
  const float time = SDL_GetTicks() / 1000.0f;

  // Cycles between true and false every couple seconds
  bool even = (fmod(time, 2.0f) < 0.99f);

#ifdef USE_IMGUI
  ImGui::Begin("Render Stats");
  ImGui::Text("FPS: %dhz\n", m_framerate.m_fps);
  ImGui::Text("Cam pos: %f %f %f\n", m_camera->m_pos.x, m_camera->m_pos.y, m_camera->m_pos.z);
  ImGui::Text("Cam rot: %f %f\n", m_camera->m_rot.x, m_camera->m_rot.y);
  ImGui::End();
#endif

  // Update miku
  if (m_miku->m_enabled) {
    m_miku->m_rot = glm::rotate(glm::quat(), -3.14159f * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
    m_miku->m_scale = glm::vec3(0.05f, 0.05f, 0.05f) * (1.0f+0.2f*sin(time));
    m_miku->dirtyLocal();
  }

  // Update camera
  auto modState = SDL_GetModState();
  auto* keyStates = SDL_GetKeyboardState(nullptr);
  m_camera->update(0.016f, (modState & KMOD_CTRL ? nullptr : keyStates));

  // Render scenegraph
  hs::RenderParams params;
  params.m_time = time;
  m_scenegraph->render(*window->renderer(), *m_camera, params);

  // Update fps counter
  m_framerate.m_frames++;
  if (time - m_framerate.m_lastUpdate > 1.0f) {
    m_framerate.m_lastUpdate = time;
    m_framerate.m_fps = m_framerate.m_frames;
    m_framerate.m_frames = 0;
  }
}