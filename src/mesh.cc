#include "compound/mesh.hh"

namespace compound {
const Vertex::Descriptor MeshVertex::getDescriptor() const {
    Descriptor descriptor;
    descriptor.bindingDescriptor.stride = sizeof(MeshVertex);
    descriptor.bindingDescriptor.divisor = 0;

    std::vector<Vertex::AttributeDescriptor> attributeDescriptors(
        3, AttributeDescriptor());

    attributeDescriptors[0].location = 0;
    attributeDescriptors[0].size = 3;
    attributeDescriptors[0].type = Vertex::AttributeDescriptor::Type::FLOAT;
    attributeDescriptors[0].normalized = false;
    attributeDescriptors[0].offset = offsetof(MeshVertex, pos);

    attributeDescriptors[1].location = 1;
    attributeDescriptors[1].size = 4;
    attributeDescriptors[1].type = Vertex::AttributeDescriptor::Type::FLOAT;
    attributeDescriptors[1].normalized = false;
    attributeDescriptors[1].offset = offsetof(MeshVertex, color);

    descriptor.attributeDescriptors = attributeDescriptors;
    return descriptor;
}

size_t MeshVertex::size() const {
    return sizeof(MeshVertex);
}
} // namespace compound