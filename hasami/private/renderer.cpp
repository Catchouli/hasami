#include "renderer.hpp"

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

}