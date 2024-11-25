#include "compound/object.hh"

#include <glm/gtx/transform.hpp>

namespace compound {

Object::Object(const glm::vec3& position, const glm::vec3& rotation,
               const glm::vec3& scale, std::shared_ptr<Mesh> pMesh,
               std::shared_ptr<Texture> pTexture)
    : _position(position),
      _rotation(rotation),
      _scale(scale),
      _pMesh(pMesh),
      _pTexture(pTexture) {
}

std::weak_ptr<Mesh> Object::mesh() const {
    return _pMesh;
}

std::weak_ptr<Texture> Object::texture() const {
    return _pTexture;
}

glm::mat4 Object::modelMatrix() const {
    return glm::translate(_position) *
           glm::rotate(_rotation[2], glm::vec3{0.0f, 0.0f, 1.0f}) *
           glm::rotate(_rotation[1], glm::vec3{0.0f, 1.0f, 0.0f}) *
           glm::rotate(_rotation[0], glm::vec3{1.0f, 0.0f, 0.0f}) *
           glm::scale(_scale);
}

const Vertex::Descriptor ObjectVertex::getDescriptor() const {
    Descriptor descriptor;
    descriptor.bindingDescriptor.stride = sizeof(ObjectVertex);
    descriptor.bindingDescriptor.divisor = 1;

    std::vector<Vertex::AttributeDescriptor> attributeDescriptors(
        5, AttributeDescriptor());

    attributeDescriptors[0].location = 3;
    attributeDescriptors[0].size = 4;
    attributeDescriptors[0].type = Vertex::AttributeDescriptor::Type::FLOAT;
    attributeDescriptors[0].normalized = false;
    attributeDescriptors[0].offset = offsetof(ObjectVertex, model);

    attributeDescriptors[1].location = 4;
    attributeDescriptors[1].size = 4;
    attributeDescriptors[1].type = Vertex::AttributeDescriptor::Type::FLOAT;
    attributeDescriptors[1].normalized = false;
    attributeDescriptors[1].offset =
        offsetof(ObjectVertex, model) + 4 * sizeof(float);

    attributeDescriptors[2].location = 5;
    attributeDescriptors[2].size = 4;
    attributeDescriptors[2].type = Vertex::AttributeDescriptor::Type::FLOAT;
    attributeDescriptors[2].normalized = false;
    attributeDescriptors[2].offset =
        offsetof(ObjectVertex, model) + 8 * sizeof(float);

    attributeDescriptors[3].location = 6;
    attributeDescriptors[3].size = 4;
    attributeDescriptors[3].type = Vertex::AttributeDescriptor::Type::FLOAT;
    attributeDescriptors[3].normalized = false;
    attributeDescriptors[3].offset =
        offsetof(ObjectVertex, model) + 12 * sizeof(float);

    attributeDescriptors[4].location = 7;
    attributeDescriptors[4].size = 3;
    attributeDescriptors[4].type = Vertex::AttributeDescriptor::Type::FLOAT;
    attributeDescriptors[4].normalized = false;
    attributeDescriptors[4].offset = offsetof(ObjectVertex, atlasCoords);

    descriptor.attributeDescriptors = attributeDescriptors;
    return descriptor;
}

size_t ObjectVertex::size() const {
    return sizeof(ObjectVertex);
}
} // namespace compound