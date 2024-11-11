#pragma once

#include "glm/glm.hpp"
#include "mesh.hh"

namespace compound {
struct alignas(16) ObjectVertex : public Vertex {
public:
    ObjectVertex() {
    }
    ObjectVertex(glm::mat4 model) : model(model) {
    }
    const Descriptor getDescriptor() const override;
    size_t size() const override;
    glm::mat4 model;
};

class Object {
public:
    Object(const glm::vec3& position, const glm::vec3& rotation,
           const glm::vec3& scale, std::shared_ptr<Mesh> mesh);
    glm::mat4 modelMatrix() const;
    std::weak_ptr<Mesh> mesh() const;

private:
    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;
    std::weak_ptr<Mesh> _pMesh;
};
} // namespace compound