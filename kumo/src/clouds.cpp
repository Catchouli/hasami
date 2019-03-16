#include "clouds.hpp"

#include "stb_perlin.h"

class CloudMaterial
  : public hs::StandardMaterial
{
public:
  hs::SamplerT<hs::UniformType::Sampler3D> noiseTex = {"sampler_noiseTex"};
  
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
    int noiseResolution = 64;
    std::vector<float> data;
    data.reserve(noiseResolution * noiseResolution * noiseResolution);
    for (int z = 0; z < noiseResolution; ++z) {
      for (int y = 0; y < noiseResolution; ++y) {
        for (int x = 0; x < noiseResolution; ++x) {
          const float scale = 0.01f;
          const float lacunarity = 8.0f;
          const float gain = 0.5f;
          const int octaves = 5;
          data.push_back(stb_perlin_fbm_noise3(x * scale, y * scale, z * scale, lacunarity, gain, octaves));
        }
      }
    }
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
