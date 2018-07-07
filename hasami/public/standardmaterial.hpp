#pragma once
#include "renderer.hpp"
#include "material.hpp"

namespace hs
{

class StandardMaterial
  : public Material
{
public:
  ShaderVarT<float> time = {"uni_time", 0.0f};
  ShaderVarT<glm::mat4> m = {"uni_m", glm::mat4()};
  ShaderVarT<glm::mat4> mv = {"uni_mv", glm::mat4()};
  ShaderVarT<glm::mat4> mvp = {"uni_mvp", glm::mat4()};
  Sampler albedo = {"sampler_albedo"};

  StandardMaterial(hs::Renderer* renderer, const char* shaderPath)
    : Material(renderer, shaderPath)
  {
    shader()->addAttrib("in_pos", hs::AttribType::Vec3);
    shader()->addAttrib("in_nrm", hs::AttribType::Vec3);
    shader()->addAttrib("in_uv", hs::AttribType::Vec2);

    addUniforms({&time, &m, &mv, &mvp});
    addSamplers({&albedo});
  }
};

}