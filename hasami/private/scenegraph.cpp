#include "scenegraph.hpp"

#include "gtx/quaternion.hpp"
#include "gtc/matrix_transform.hpp"

namespace hs {

SceneNode::SceneNode()
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::setParent(std::shared_ptr<SceneNode> newParent)
{
  if (m_parent)
    m_parent->removeChild(ptr());
  m_parent = newParent;
  if (m_parent)
    m_parent->addChild(ptr());
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
{
}

void AssemblyNode::draw(Renderer& renderer, Shader& shader, const glm::mat4& projection, const glm::mat4& parentmv)
{
  updateTransform();
  glm::mat4 modelview = parentmv * m_local;

  for (auto child : children()) {
    child->draw(renderer, shader, projection, modelview);
  }
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

void ModelNode::draw(Renderer& renderer, Shader& shader, const glm::mat4& projection, const glm::mat4& modelview)
{
  m_mesh->draw(renderer, shader, projection, modelview);
}

}