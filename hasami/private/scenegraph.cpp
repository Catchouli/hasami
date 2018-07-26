#include "scenegraph.hpp"

#include "gtx/quaternion.hpp"
#include "gtc/matrix_transform.hpp"

#include "mesh.hpp"

namespace hs {

SceneNode::SceneNode()
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::addState(const RenderState& state)
{
  m_stateset[state.m_state] = state;
}

void SceneNode::removeState(const RenderState& state)
{
  m_stateset.erase(state.m_state);
}

void SceneNode::setParent(std::shared_ptr<SceneNode> newParent)
{
  if (m_parent)
    m_parent->removeChild(ptr());
  m_parent = newParent;
  if (m_parent)
    m_parent->addChild(ptr());
}

void SceneNode::render(Renderer& renderer, const Camera& camera, const RenderParams& params)
{
  // Push states
  pushStates(renderer);

  // Clear screen
  if (params.m_clearColorBuf || params.m_clearDepthBuf)
    renderer.clear(params.m_clearColorBuf, params.m_clearDepthBuf);

  // Construct context
  hs::Context ctx;
  ctx.m_time = params.m_time;
  ctx.m_projection = camera.projMat();
  ctx.m_view = camera.viewMat();
  ctx.m_viewInv = glm::inverse(ctx.m_view);
  ctx.m_object = glm::mat4();

  // Draw scenegraph
  draw(renderer, ctx);

  // Pop states
  popStates(renderer);
}

void SceneNode::pushStates(Renderer& renderer)
{
  auto* stateManager = renderer.stateManager();
  for (const auto& state : m_stateset) {
    stateManager->pushState(state.second);
  }
}

void SceneNode::popStates(Renderer& renderer)
{
  auto* stateManager = renderer.stateManager();
  for (const auto& state : m_stateset) {
    stateManager->popState(state.second);
  }
}

void SceneNode::addChild(std::shared_ptr<SceneNode> node)
{
  if (m_children.find(node) == m_children.end()) {
    m_children.insert(node);
  }
}

void SceneNode::removeChild(std::shared_ptr<SceneNode> node)
{
  auto it = m_children.find(node);
  if (it != m_children.end()) {
    m_children.erase(it);
  }
}

AssemblyNode::AssemblyNode()
  : m_localDirty(false)
  , m_scale(1.0f, 1.0f, 1.0f)
  , m_enabled(true)
{
}

void AssemblyNode::draw(Renderer& renderer, const Context& ctx)
{
  if (!m_enabled)
    return;

  // Push states
  pushStates(renderer);

  // Update transform if dirty
  updateTransform();

  // Update object transform
  Context newCtx = ctx;
  newCtx.m_object = ctx.m_object * m_local;

  // Traverse into children
  for (auto child : children()) {
    child->draw(renderer, newCtx);
  }

  // Pop states
  popStates(renderer);
}

void AssemblyNode::updateTransform()
{
  if (!m_localDirty)
    return;

  m_localDirty = false;

  glm::mat4 translation = glm::translate(glm::mat4(), m_pos);
  glm::mat4 rotation = glm::toMat4(m_rot);
  glm::mat4 scale = glm::scale(glm::mat4(), m_scale);
  m_local = translation * rotation * scale;
}

void ModelNode::draw(Renderer& renderer, const Context& ctx)
{
  if (m_mesh && m_mat) {
    m_mesh->draw(renderer, *m_mat, ctx);
  }
}

}