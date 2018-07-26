#pragma once

#include <stdint.h>
#include <vec3.hpp>
#include <vec2.hpp>
#include <matrix.hpp>

namespace hs {

class Camera
{
public:
  virtual const glm::mat4& projMat() const = 0;
  virtual const glm::mat4& viewMat() const = 0;
};

class FPSCamera
  : public Camera
{
public:
  FPSCamera();
  void mouseMove(int x, int y);
  void update(float timeDelta, const uint8_t* keyStates);

  const glm::mat4& projMat() const override { return m_proj; }
  const glm::mat4& viewMat() const override { return m_view; }

  float m_camSpeed;
  float m_camSensitivity;

  glm::vec3 m_pos;
  glm::vec2 m_rot;
  bool m_lockCamera;

private:
  glm::mat4 m_proj;
  glm::mat4 m_view;

  glm::vec3 m_forward;
  glm::vec3 m_left;
  glm::vec3 m_up;
};

}