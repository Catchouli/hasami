#include "util/cached_vector.hpp"
#include "clouds.hpp"

#include "stb_perlin.h"
#include <functional>
#include <cstdio>
#include "renderer/material.hpp"
#include "renderer/mesh.hpp"

class CloudMaterial
  : public hs::StandardMaterial
{
public:
  hs::SamplerT<hs::UniformType::Sampler3D> noiseTex = { "sampler_noiseTex" };

  CloudMaterial(hs::Renderer* renderer, const char* shaderPath)
    : StandardMaterial(renderer, shaderPath)
  {
    addSampler(&noiseTex);
  }
};

CloudsNode::CloudsNode(hs::Renderer& renderer)
{
  using namespace hs;

  // Create mesh
  m_mesh = std::make_shared<Mesh>(renderer);

  m_mesh->m_attrib.clear();
  m_mesh->m_attrib.push_back(Attrib(Attrib_pos, 3, AttribType::Float));
  m_mesh->m_attrib.push_back(Attrib(Attrib_nrm, 3, AttribType::Float));
  m_mesh->m_attrib.push_back(Attrib(Attrib_tex0, 2, AttribType::Float));

  // Create screen space quad
  std::vector<Vertex> vert = {
    Vertex{ { -1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
    Vertex{ {  1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
    Vertex{ {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
    Vertex{ {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
    Vertex{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
    Vertex{ { -1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
  };
  m_mesh->m_buf->set(vert, sizeof(Vertex), hs::BufferUsage::StaticDraw);

  // Create noise texture
  auto noise = std::shared_ptr<hs::Texture>(renderer.createTexture());
  {
    const int noiseResolution = 128;

    std::vector<float> data;
    cacheVector<float>("res/cloud_shape_cache", 1, data, [noiseResolution](std::vector<float>& data) {
      const int noiseScale = 16;
      const int noiseWrap = noiseResolution / noiseScale;
      const float coordScale = 1.0f / static_cast<float>(noiseScale);

      const float scale = 16.0f / 128.0f;
      const float lacunarity = 2.0f;
      const float gain = 0.5f;
      const int octaves = 8;

      data.reserve(noiseResolution * noiseResolution * noiseResolution);
      for (int z = 0; z < noiseResolution; ++z) {
        for (int y = 0; y < noiseResolution; ++y) {
          for (int x = 0; x < noiseResolution; ++x) {
            glm::vec3 p = coordScale * glm::vec3(float(x), float(y), float(z));
            float noise = stb_perlin_noise3(p.x, p.y, p.z, noiseWrap, noiseWrap, noiseWrap);
            data.push_back(noise);
          }
        }
      }
    });
    noise->set(TextureFormat::R32F, noiseResolution, noiseResolution, noiseResolution, data.data());
  }

  // Create material
  auto cloudMaterial = std::make_shared<CloudMaterial>(&renderer, "res/clouds.glsl");
  cloudMaterial->noiseTex.set(noise);
  m_mat = cloudMaterial;
}

void CloudsNode::draw(hs::Renderer& renderer, const hs::Context& ctx)
{
  ModelNode::draw(renderer, ctx);
}
