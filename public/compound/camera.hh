#pragma once

#include "glm/glm.hpp"

namespace compound {
class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 axis, glm::vec3 forward,
           float nearClip, float farClip, float horizontalFov);
    glm::mat4 projection(float ratio) const;
    glm::mat4 view() const;
    void move(glm::vec3 move);
    void rotate(float yaw, float pitch, float roll);
    const glm::vec3 forward() const {
        return _forward;
    }
    const glm::vec3 left() const {
        return glm::cross(_up, _forward);
    }
    const glm::vec3 up() const {
        return _up;
    }

private:
    glm::vec3 _position;
    glm::vec3 _axis;
    glm::vec3 _up;
    glm::vec3 _forward;
    float _nearClip;
    float _farClip;
    float _horizontalFov;
};
}