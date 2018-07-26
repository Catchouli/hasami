#pragma once

#include "util/variant.hpp"
#include "vec4.hpp"

namespace hs {

/// Render state API
/// Example usage:
///   renderer()->stateManager()->pushState(ClearColor(0.0f, 0.0f, 0.0f, 1.0f));
///   renderer()->stateManager()->pushState(ClearColor());

/// RenderState structure
/// Users do not need to use this directly except to access the sealed enums.
/// This is data-driven rather than in the type system so it can be safely passed
/// through interfaces. To add a new state update RenderState::State and add constructors
/// for it below. Additional sealed enums or types to the Value variant may be needed.
struct RenderState
{
  enum class State { ClearColor, DepthTest, CullFace, PolygonMode, AlphaBlend };
  enum class PolygonMode { Point, Line, Fill };
  using Value = std::variant<glm::vec4, bool, PolygonMode>;

  State m_state;
  Value m_val;
};

inline RenderState ClearColor() { return RenderState{RenderState::State::ClearColor}; };
inline RenderState ClearColor(const glm::vec4& v) { return RenderState{RenderState::State::ClearColor, v}; };
inline RenderState DepthTest() { return RenderState{RenderState::State::DepthTest}; };
inline RenderState DepthTest(bool b) { return RenderState{RenderState::State::DepthTest, b}; };
inline RenderState CullFace() { return RenderState{RenderState::State::CullFace}; };
inline RenderState CullFace(bool b) { return RenderState{RenderState::State::CullFace, b}; };
inline RenderState PolygonMode() { return RenderState{RenderState::State::PolygonMode}; };
inline RenderState PolygonMode(RenderState::PolygonMode mode) { return RenderState{RenderState::State::PolygonMode, mode}; };
inline RenderState AlphaBlend() { return RenderState{RenderState::State::AlphaBlend}; };
inline RenderState AlphaBlend(bool b) { return RenderState{RenderState::State::AlphaBlend, b}; };

}