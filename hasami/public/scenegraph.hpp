#pragma once

#include <set>
#include <memory>

#include "gl/shader.hpp"
#include "mesh.hpp"

#include "matrix.hpp"
#include "vec3.hpp"
#include "gtc/quaternion.hpp"

namespace hs {

class StandardShader
  : public hs::gl::Shader
{
public:
  StandardShader(const char* srcPath)
    : Shader(srcPath
      , VarRef("mv", _mv)
      , VarRef("mvp", _mvp)
      )
  {}

  ShaderVar<glm::mat4> _mv;
  ShaderVar<glm::mat4> _mvp;
};

class SceneNode
  : public std::enable_shared_from_this<SceneNode>
{
public:
  SceneNode();
  ~SceneNode();

  void setParent(std::shared_ptr<SceneNode> newParent);
  std::shared_ptr<SceneNode> parent() { return m_parent; }

  const std::set<std::shared_ptr<SceneNode>>& children() { return m_children; }

  virtual void draw(gl::Shader& shader, const glm::mat4& projection, const glm::mat4& view) = 0;

  std::shared_ptr<SceneNode> ptr() { return shared_from_this(); }

private:
  void addChild(std::shared_ptr<SceneNode> node);
  void removeChild(std::shared_ptr<SceneNode> node);

  std::shared_ptr<SceneNode> m_parent;
  std::set<std::shared_ptr<SceneNode>> m_children;
};

class AssemblyNode
  : public SceneNode
{
public:
  AssemblyNode();

  virtual void draw(gl::Shader& shader, const glm::mat4& projection, const glm::mat4& view) override;

  void dirtyLocal() { m_localDirty = true; }

  glm::vec3 m_pos;
  glm::quat m_rot;
  glm::vec3 m_scale;

private:
  void updateTransform();

  bool m_localDirty;
  glm::mat4 m_local;
};

class ModelNode
  : public SceneNode
{
public:
  virtual void draw(gl::Shader& shader, const glm::mat4& projection, const glm::mat4& view) override;

  std::shared_ptr<Mesh> m_mesh;
};

}