#include "camera.hpp"

#include <gtc/matrix_transform.hpp>
#include <SDL.h>

namespace hs {

FPSCamera::FPSCamera()
  : m_forward(0.0, 0.0, -1.0)
  , m_left(-1.0, 0.0, 0.0)
  , m_up(0.0, 1.0, 0.0)
  , m_camSpeed(1.0f)
  , m_camSensitivity(1.0f)
{
}

void FPSCamera::mouseMove(int x, int y)
{
  m_rot.y += x;
  m_rot.x += y;
}

void FPSCamera::update(float timeDelta, const uint8_t* keyStates)
{
  glm::mat4 camRotX = glm::rotate(glm::mat4(), m_rot.x * m_camSensitivity, glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 camRotY = glm::rotate(glm::mat4(), m_rot.y * m_camSensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 camRot = camRotX * camRotY;

  m_forward = glm::vec3(0.0f, 0.0f, -1.0f) * glm::mat3(camRot);
  m_left = glm::vec3(-1.0f, 0.0f, 0.0f) * glm::mat3(camRot);
  m_up = glm::vec3(0.0f, 1.0f, 0.0f) * glm::mat3(camRot);

  if (keyStates[SDL_SCANCODE_W]) {
    m_pos += m_forward * m_camSpeed;
  }
  if (keyStates[SDL_SCANCODE_S]) {
    m_pos -= m_forward * m_camSpeed;
  }
  if (keyStates[SDL_SCANCODE_A]) {
    m_pos += m_left * m_camSpeed;
  }
  if (keyStates[SDL_SCANCODE_D]) {
    m_pos -= m_left * m_camSpeed;
  }
  if (keyStates[SDL_SCANCODE_E]) {
    m_pos += m_up * m_camSpeed;
  }
  if (keyStates[SDL_SCANCODE_Q]) {
    m_pos -= m_up * m_camSpeed;
  }

  m_proj = glm::perspective(3.141f / 2.0f, 1.0f, 0.001f, 50.0f);
  m_view = camRot * glm::translate(glm::mat4(), -m_pos);
}

}