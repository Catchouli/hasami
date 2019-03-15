#pragma once

#include <set>
#include <map>
#include <memory>

#include "camera.hpp"
#include "matrix.hpp"
#include "vec3.hpp"
#include "gtc/quaternion.hpp"
#include "backends/gl/renderstate.hpp"

namespace hs {
  class Renderer;
  struct RenderState;

  class Mesh;
class StandardMaterial;

struct RenderParams
{
  float m_time = 0.0f;
  bool m_clearColorBuf = true;
  bool m_clearDepthBuf = true;
};

struct Context
{
  float m_time = 0.0f;
  glm::mat4 m_projection;
  glm::mat4 m_view;
  glm::mat4 m_viewInv;
  glm::mat4 m_object;
};

class SceneNode
  : public std::enable_shared_from_this<SceneNode>
{
public:
  SceneNode();
  ~SceneNode();
  
  /// Add a state to the stateset
  void addState(const RenderState& state);

  /// Remove a state from the stateset
  void removeState(const RenderState& state);

  void setParent(std::shared_ptr<SceneNode> newParent);
  std::shared_ptr<SceneNode> parent() { return m_parent; }

  const std::set<std::shared_ptr<SceneNode>>& children() { return m_children; }

  /// Render the scene from the given camera with the given parameters
  void render(Renderer& renderer, const Camera& camera, const RenderParams& params);

  virtual void draw(Renderer& renderer, const Context& ctx) = 0;

  std::shared_ptr<SceneNode> ptr() { return shared_from_this(); }

protected:
  void pushStates(Renderer& renderer);
  void popStates(Renderer& renderer);

private:
  void addChild(std::shared_ptr<SceneNode> node);
  void removeChild(std::shared_ptr<SceneNode> node);

  std::shared_ptr<SceneNode> m_parent;
  std::set<std::shared_ptr<SceneNode>> m_children;

  std::map<hs::RenderState::State, hs::RenderState> m_stateset;
};

class AssemblyNode
  : public SceneNode
{
public:
  AssemblyNode();

  /// Dirty the local transform to have it update next time it's needed
  void dirtyLocal() { m_localDirty = true; }

  /// SceneNode::draw
  virtual void draw(Renderer& renderer, const Context& ctx) override;

  bool m_enabled;
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
  virtual void draw(Renderer& renderer, const Context& ctx) override;

  std::shared_ptr<Mesh> m_mesh;
  std::shared_ptr<StandardMaterial> m_mat;
};

}
