#define FREEIMAGE_LIB
#include "FreeImage.h"
#include "Utilities.h"
#include "backends/gl/renderer.hpp"

namespace hs {

StateManager::StateManager()
{
  pushState(DepthTest(false));
  pushState(CullFace(false));
  pushState(ClearColor(glm::vec4(0.0f)));
  pushState(PolygonMode(RenderState::PolygonMode::Fill));
}

void StateManager::pushState(RenderState renderState)
{
  auto& stack = m_stacks[renderState.m_state];
  stack.push(renderState);
  m_dirtyStates.insert(&stack);
}

void StateManager::popState(RenderState renderState)
{
  auto& stack = m_stacks[renderState.m_state];
  stack.pop();
  m_dirtyStates.insert(&stack);
}

void StateManager::flush()
{
  for (const auto& state : m_dirtyStates) {
    if (!state->empty()) {
      applyState(state->top());
    }
  }
}

bool Texture::load(const char* path)
{
  // Initialise freeimage
  FreeImage_Initialise();

  // Attempt to determine format
  auto format = FreeImage_GetFIFFromFilename(path);
  if (format == FIF_UNKNOWN) {
    fprintf(stderr, "Failed to determine image format for %s\n", path);
    return false;
  }

  FIBITMAP* bmp = FreeImage_Load(format, path);
  if (!bmp) {
    fprintf(stderr, "Failed to load %s\n", path);
    return false;
  }

  FIBITMAP* bmp32 = FreeImage_ConvertTo32Bits(bmp);
  FreeImage_Unload(bmp);
  if (!bmp32) {
    return false;
  }

  if (!SwapRedBlue32(bmp32)) {
    fprintf(stderr, "Failed to swap red/blue channels for %s\n", path);
  }

  // Get texture data
  void* bits = FreeImage_GetBits(bmp32);
  int width = FreeImage_GetWidth(bmp32);
  int height = FreeImage_GetHeight(bmp32);

  // Set texture
  set(TextureFormat::RGBA8888, width, height, bits);

  // Unload bitmap
  FreeImage_Unload(bmp32);

  return true;
}

}
