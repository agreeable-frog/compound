#pragma once

#include "glm/glm.hpp"
#include "mesh.hh"
#include "texture_atlas.hh"

namespace compound {
struct alignas(16) ObjectVertex : public Vertex {
public:
    ObjectVertex() {
    }
    ObjectVertex(glm::mat4 model, glm::vec3 atlasCoords)
        : model(model), atlasCoords(atlasCoords) {
    }
    const Descriptor getDescriptor() const override;
    size_t size() const override;
    glm::mat4 model;
    glm::vec3 atlasCoords;
};

class Object {
public:
    Object(const glm::vec3& position, const glm::vec3& rotation,
           const glm::vec3& scale, std::shared_ptr<Mesh> mesh, std::shared_ptr<Texture> texture);
    glm::mat4 modelMatrix() const;
    std::weak_ptr<Mesh> mesh() const;
    std::weak_ptr<Texture> texture() const;

private:
    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;
    std::weak_ptr<Mesh> _pMesh;
    std::weak_ptr<Texture> _pTexture;
};
} // namespace compound