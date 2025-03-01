#include "compound/camera.hh"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace compound {
Camera::Camera(glm::vec3 position, glm::vec3 axis, glm::vec3 forward,
               float nearClip, float farClip, float horizontalFov)
    : _position(position),
      _axis(axis),
      _up(axis),
      _forward(forward),
      _nearClip(nearClip),
      _farClip(farClip),
      _horizontalFov(horizontalFov) {
}

glm::mat4 Camera::projection(float ratio) const {
    return glm::perspective(_horizontalFov, ratio, _nearClip, _farClip);
}

glm::mat4 Camera::view() const {
    return glm::lookAt(_position, _position + _forward, _up);
}

void Camera::move(glm::vec3 move) {
    _position += move;
}

void Camera::rotate(float yaw, float pitch, float roll) {
    glm::mat4 yawRot = glm::rotate(glm::mat4(1.0f), yaw, _axis);
    _forward = yawRot * glm::vec4(_forward, 1.0f);
    _up = yawRot * glm::vec4(_up, 1.0f);
    glm::mat4 pitchRot =
        glm::rotate(glm::mat4(1.0f), pitch, glm::cross(_forward, _up));
    _forward = pitchRot * glm::vec4(_forward, 1.0f);
    _up = pitchRot * glm::vec4(_up, 1.0f);
    glm::mat4 rollRot = glm::rotate(glm::mat4(1.0f), roll, _forward);
    _up = rollRot * glm::vec4(_up, 1.0f);
}
} // namespace compound