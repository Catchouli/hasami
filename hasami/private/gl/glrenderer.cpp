#include "gl/glrenderer.hpp"
#include "gl/glshader.hpp"
#include "gl/glbuffer.hpp"
#include "gl/gltexture.hpp"

#include "glad/glad.h"

#include "variant.hpp"

namespace hs {
namespace gl {

int glPrimType(PrimitiveType prim) {
  switch (prim) {
    case PrimitiveType::Triangles: return GL_TRIANGLES;
    default: assert(false); return 0;
  }
}


GLRenderer::GLRenderer()
{
  if (!gladLoadGL()) {
    fprintf(stderr, "Glad (opengl) failed to initialise\n");
    throw;
  }

  m_stateManager = std::make_shared<gl::StateManager>();
}

hs::Shader* GLRenderer::createShader()
{
  return new gl::Shader();
}

hs::Buffer* GLRenderer::createBuffer()
{
  return new gl::Buffer();
}

hs::Texture* GLRenderer::createTexture()
{
  return new gl::Texture();
}

void GLRenderer::drawArrays(PrimitiveType prim, int start, int count)
{
  glDrawArrays(glPrimType(prim), start, count);
}

void GLRenderer::clear(bool color, bool depth)
{
  glClear((color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0));
}

bool GLRenderer::checkError()
{
  #ifdef _DEBUG
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    fprintf(stderr, "GL error: %d\n", err);
    return true;
  }
  #endif
  return false;
}

GLenum glPolyMode(RenderState::PolygonMode mode) {
  switch (mode) {
    case RenderState::PolygonMode::Point: return GL_POINT;
    case RenderState::PolygonMode::Line: return GL_LINE;
    case RenderState::PolygonMode::Fill: return GL_FILL;
    default: assert(false); return 0;
  }
}

void StateManager::applyState(const RenderState& renderState)
{
  switch (renderState.m_state) {
    case RenderState::State::DepthTest: { (mpark::get<bool>(renderState.m_val) ? glEnable : glDisable)(GL_DEPTH_TEST); } break;
    case RenderState::State::CullFace: { (mpark::get<bool>(renderState.m_val) ? glEnable : glDisable)(GL_CULL_FACE); } break;
    case RenderState::State::ClearColor: { const auto& v = mpark::get<glm::vec4>(renderState.m_val); glClearColor(v.x, v.y, v.z, v.w); } break;
    case RenderState::State::PolygonMode: { glPolygonMode(GL_FRONT_AND_BACK, glPolyMode(mpark::get<RenderState::PolygonMode>(renderState.m_val))); } break;
    default: assert(false); break;
  }
}

}
}