#pragma once
#include "renderer.hpp"
#include "material.hpp"
#include "constants.hpp"

namespace hs
{

class StandardMaterial
  : public Material
{
public:
  ShaderVarT<float> time = {"uni_time", 0.0f};
  ShaderVarT<glm::mat4> model = {"uni_model", glm::mat4()};
  ShaderVarT<glm::mat4> view = {"uni_view", glm::mat4()};
  ShaderVarT<glm::mat4> projection = {"uni_projection", glm::mat4()};
  ShaderVarT<glm::mat4> mvp = {"uni_mvp", glm::mat4()};
  SamplerT<hs::UniformType::Sampler2D> albedo = {"sampler_albedo"};

  StandardMaterial(hs::Renderer* renderer, const char* shaderPath)
    : Material(renderer, shaderPath)
  {
    shader()->addAttrib(Attrib_pos, hs::AttribType::Vec3);
    shader()->addAttrib(Attrib_nrm, hs::AttribType::Vec3);
    shader()->addAttrib(Attrib_tex0, hs::AttribType::Vec2);
    shader()->addAttrib(Attrib_col, hs::AttribType::Float);

    addUniforms({&time, &model, &view, &projection, &mvp});
    addSamplers({&albedo});
  }
};

}