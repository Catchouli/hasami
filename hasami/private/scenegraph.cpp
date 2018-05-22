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

void AssemblyNode::draw(Renderer& renderer, Shader& shader, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& parent)
{
  updateTransform();
  glm::mat4 object = parent * m_local;

  for (auto child : children()) {
    child->draw(renderer, shader, projection, view, object);
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

void ModelNode::draw(Renderer& renderer, Shader& shader, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& object)
{
  // todo: acquire samplers
  // todo: abstract this somehow, it should really be done by the same code that's in Mesh::Draw
  //       it needs some Material type to manage the shader and what uniforms it has and what's bound
  if (m_tex) {
    TextureUnit unit = TextureUnit::Texture0;
    m_tex->bind(unit);
    bool err = renderer.checkError();
    shader.bind();
    shader.setUniform("sampler_diffuse", UniformType::Sampler2D, &unit);
    bool err2 = renderer.checkError();
  }
  if (m_mesh) {
    m_mesh->draw(renderer, shader, projection, view, object);
  }
}

}