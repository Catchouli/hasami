#include "gl/sdlwindow.hpp"
#include "SDL.h"
#include <stdio.h>
#include "renderer.hpp"
#include "mesh.hpp"
#include "gtc/matrix_transform.hpp"

#ifdef USE_IMGUI
#include "imgui.hpp"
#endif

namespace hs {
namespace sdl {
namespace internal {

SDLWindowBase::SDLWindowBase(bool createGLContext)
  : m_captureMouse(false)
  , m_mouseButtonDown()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    fprintf(stderr, "SDL error: %s\n", SDL_GetError());
    throw;
  }

  if (SDL_CreateWindowAndRenderer(800, 800, SDL_WINDOW_OPENGL, &m_win, &m_renderer) < 0) {
    fprintf(stderr, "SDL error: %s\n", SDL_GetError());
    throw;
  }

  if (createGLContext) {
    SDL_GL_CreateContext(m_win);
  }
}

SDLWindowBase::~SDLWindowBase()
{
  imguiShutdown();
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_win);
}

void SDLWindowBase::init()
{
  // initialise imgui
  imguiInit();
}

void SDLWindowBase::run()
{
  while (m_app && m_app->running()) {
    SDL_Event evt;
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) {
        setApp(nullptr);
      }

      if (evt.type == SDL_MOUSEBUTTONDOWN && m_captureMouse) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_SetWindowGrab(m_win, SDL_TRUE);
      }

      if (evt.type == SDL_MOUSEBUTTONDOWN || evt.type == SDL_MOUSEBUTTONUP) {
        if (evt.button.button == SDL_BUTTON_LEFT) {
          m_mouseButtonDown[0] = (evt.button.state == SDL_PRESSED);
        }
        else if (evt.button.button == SDL_BUTTON_RIGHT) {
          m_mouseButtonDown[1] = (evt.button.state == SDL_PRESSED);
        }
      }

      #ifdef USE_IMGUI
      auto& io = ImGui::GetIO();
      switch (evt.type) {
        case SDL_MOUSEWHEEL:
        {
          if (evt.wheel.x > 0) io.MouseWheelH += 1;
          if (evt.wheel.x < 0) io.MouseWheelH -= 1;
          if (evt.wheel.y > 0) io.MouseWheel += 1;
          if (evt.wheel.y < 0) io.MouseWheel -= 1;
          break;
        }
        case SDL_TEXTINPUT:
        {
          io.AddInputCharactersUTF8(evt.text.text);
          break;
        }
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
          int key = evt.key.keysym.scancode;
          IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
          io.KeysDown[key] = (evt.type == SDL_KEYDOWN);
          io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
          io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
          io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
          io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
          break;
        }
      };
      #endif

      if (m_app) {
        m_app->input(&evt);
      }
    }

    if (m_app) {
      imguiNewFrame();
      m_app->render(this);
      imguiRender();
      SDL_GL_SwapWindow(m_win);
      if (auto* renderer = this->renderer())
        renderer->checkError();
    }
  }
}

void SDLWindowBase::imguiInit()
{
#ifdef USE_IMGUI
  ImGui::CreateContext();
  if (auto* renderer = this->renderer()) {
    auto& io = ImGui::GetIO();

    int width, height;
    unsigned char* pixels = nullptr;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    m_fontAtlas = std::shared_ptr<hs::Texture>(renderer->createTexture());
    m_fontAtlas->set(hs::TextureFormat::RGBA8888, width, height, pixels);
  }
#endif
}

void SDLWindowBase::imguiShutdown()
{
#ifdef USE_IMGUI
  ImGui::DestroyContext();
#endif
}

void SDLWindowBase::imguiNewFrame()
{
#ifdef USE_IMGUI
  int x, y;
  SDL_GetMouseState(&x, &y);

  auto& io = ImGui::GetIO();
  io.DeltaTime = 1.0f/60.0f;
  io.DisplaySize.x = 800.0f;
  io.DisplaySize.y = 800.0f;
  io.MousePos = ImVec2(float(x), float(y));
  io.MouseDown[0] = m_mouseButtonDown[0];
  io.MouseDown[1] = m_mouseButtonDown[1];

  ImGui::NewFrame();
#endif
}

void SDLWindowBase::imguiRender()
{
#ifdef USE_IMGUI
  ImGui::EndFrame();
  ImGui::Render();

  if (auto* renderer = this->renderer()) {
    static hs::Mesh mesh(*renderer, true);
    static std::shared_ptr<hs::StandardMaterial> mat = std::make_shared<hs::StandardMaterial>(renderer, "res/imgui.glsl");
    mesh.m_attrib.clear();
    mesh.m_attrib.push_back(hs::Attrib(Attrib_pos,  2, hs::AttribType::Float));
    mesh.m_attrib.push_back(hs::Attrib(Attrib_tex0, 2, hs::AttribType::Float));
    mesh.m_attrib.push_back(hs::Attrib(Attrib_col,  1, hs::AttribType::Float));
    mesh.m_indexFormat = hs::IndexFormat::U16;
    mat->albedo.set(m_fontAtlas);

    hs::Context ctx;
    ctx.m_time = 0.0f;
    ctx.m_projection = glm::ortho(0.0f, 800.0f, 800.0f, 0.0f);

    renderer->stateManager()->pushState(hs::PolygonMode(hs::RenderState::PolygonMode::Fill));
    renderer->stateManager()->pushState(hs::CullFace(false));
    renderer->stateManager()->pushState(hs::DepthTest(false));
    renderer->stateManager()->pushState(hs::AlphaBlend(true));

    const auto* drawData = ImGui::GetDrawData();
    for (int n = 0; n < drawData->CmdListsCount; ++n) {
      const auto* cmdList = drawData->CmdLists[n];
      mesh.m_buf->set(cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size, sizeof(ImDrawVert), hs::BufferUsage::StaticDraw);
      mesh.m_ibuf->set(cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size, sizeof(ImDrawIdx), hs::BufferUsage::StaticDraw);

      for (int i = 0; i < cmdList->CmdBuffer.Size; i++) {
        const ImDrawCmd* cmd = &cmdList->CmdBuffer[i];
        if (cmd->UserCallback) {
          cmd->UserCallback(cmdList, cmd);
        }
        else {
          mesh.m_count = cmd->ElemCount;
          mesh.draw(*renderer, *mat, ctx);
        }
      }
    }
    
    mesh.m_indexed = false;
    std::vector<float> vb = {
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      800.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      800.0f, 800.0f, 1.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      800.0f, 800.0f, 1.0f, 1.0f, 0.0f,
      0.0f, 800.0f, 0.0f, 1.0f, 0.0f,
    };
    mesh.m_buf->set(vb, 5 * (int)sizeof(float), hs::BufferUsage::StaticDraw);
    //mesh.draw(*renderer, *mat, ctx);
    mesh.m_indexed = true;

    renderer->stateManager()->popState(hs::AlphaBlend());
    renderer->stateManager()->popState(hs::PolygonMode());
    renderer->stateManager()->popState(hs::CullFace());
    renderer->stateManager()->popState(hs::DepthTest());
  }
#endif
}

}
}
}