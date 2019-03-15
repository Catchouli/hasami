#include "backends/gl/material.hpp"
#include <memory>

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
  // todo: dirty flag no longer works since shader programs are shared
  //if (m_dirty) {
    m_dirty = false;
    if (hs::Shader* shader = m_mat->shader()) {
      shader->setUniform(m_name.c_str(), valTy(), valPtr());
    }
  //}
}

/* Sampler */

Sampler::Sampler(const char* name)
  : m_var(name, TextureUnit::Texture0)
{
}

void Sampler::create(Material* mat, TextureUnit unit)
{
  m_var.create(mat);
  m_var.set(unit);
}

/* Material */

Material::Material(hs::Renderer* renderer, const char* shaderPath)
  : m_units(TextureUnit::Texture0)
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

void Material::addSampler(Sampler* sampler)
{
  sampler->create(this, m_units);
  m_samplers.push_back(sampler);
  m_units = (TextureUnit)((int)m_units+1);
  m_units = (m_units == TextureUnit::Texture_Max ? (TextureUnit)((int)TextureUnit::Texture_Max-1): m_units);
}

void Material::addSamplers(std::vector<Sampler*> samplers)
{
  for (auto& sampler : samplers) {
    addSampler(sampler);
  }
}

void Material::flush()
{
  for (auto& sampler : m_samplers) {
    if (auto* tex = sampler->tex()) {
      tex->bind(sampler->var().val());
      m_shader->setUniformEnabled(sampler->var().name(), true);
    }
    else {
      m_shader->setUniformEnabled(sampler->var().name(), false);
    }
  }
  m_shader->bind();
  for (auto& var : m_vars) {
    var->update();
  }
  for (auto& sampler : m_samplers) {
    sampler->var().update();
  }
}

}
