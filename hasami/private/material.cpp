#include "material.hpp"

namespace hs {

/* ShaderVar */

void ShaderVar::create(Material* mat)
{
  m_mat = mat;

  if (auto* shader = m_mat->shader()) {
    shader->addUniform(m_name.c_str(), valTy());
  }
}

void ShaderVar::update()
{
  assert(m_mat);
  if (m_dirty) {
    m_dirty = false;
    if (hs::Shader* shader = m_mat->shader()) {
      shader->setUniform(m_name.c_str(), valTy(), valPtr());
    }
  }
}

/* Material */

Material::Material(hs::Renderer* renderer, const char* shaderPath)
{
  m_shader = std::shared_ptr<hs::Shader>(renderer->createShader());
  m_shader->load(shaderPath);
}

void Material::addUniform(ShaderVar* var)
{
  var->create(this);
  m_vars.push_back(var);
}

void Material::addUniforms(std::vector<ShaderVar*> vars)
{
  for (auto& var : vars) {
    addUniform(var);
  }
}

void Material::flush()
{
  m_shader->bind();
  for (auto& var : m_vars) {
    var->update();
  }
}

void Material::bind()
{
  flush();
}

void Material::unbind()
{

}

}