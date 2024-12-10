#include "compound/mesh.hh"

#include <iostream>

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
    attributeDescriptors[1].size = 3;
    attributeDescriptors[1].type = Vertex::AttributeDescriptor::Type::FLOAT;
    attributeDescriptors[1].normalized = false;
    attributeDescriptors[1].offset = offsetof(MeshVertex, normal);

    attributeDescriptors[2].location = 2;
    attributeDescriptors[2].size = 2;
    attributeDescriptors[2].type = Vertex::AttributeDescriptor::Type::FLOAT;
    attributeDescriptors[2].normalized = false;
    attributeDescriptors[2].offset = offsetof(MeshVertex, uv);

    descriptor.attributeDescriptors = attributeDescriptors;
    return descriptor;
}

size_t MeshVertex::size() const {
    return sizeof(MeshVertex);
}

Mesh::Mesh() {
}

void Mesh::loadMeshes(std::vector<std::shared_ptr<Mesh>> meshes,
                      std::shared_ptr<VertexBuffer> pVertexBuffer,
                      std::shared_ptr<IndexBuffer> pIndexBuffer) {
    std::vector<MeshVertex> vertexBufferData;
    std::vector<uint32_t> indexBufferData;
    for (auto& mesh : meshes) {
        uint32_t offset = vertexBufferData.size();
        vertexBufferData.insert(vertexBufferData.end(), mesh->_vertices.begin(),
                                mesh->_vertices.end());
        auto tmpIndexBufferData = mesh->_indices;
        std::transform(tmpIndexBufferData.begin(), tmpIndexBufferData.end(),
                       tmpIndexBufferData.begin(),
                       [offset](uint32_t i) { return i + offset; });
        mesh->_indexBufferOffset = indexBufferData.size();
        indexBufferData.insert(indexBufferData.end(),
                               tmpIndexBufferData.begin(),
                               tmpIndexBufferData.end());
        mesh->_pVertexBuffer = pVertexBuffer;
        mesh->_pIndexBuffer = pIndexBuffer;
    }
    pVertexBuffer->bufferData(vertexBufferData.data(),
                              vertexBufferData.size() * MeshVertex().size(),
                              MeshVertex().getDescriptor());
    pIndexBuffer->bufferData(indexBufferData);
}

Mesh Mesh::Triangle() {
    Mesh mesh;
    mesh._vertices.push_back(compound::MeshVertex{glm::vec3{-0.5f, -0.5f, 0.0f},
                                                  glm::vec3{0.0f, 0.0f, -1.0f},
                                                  glm::vec2{0.0f, 0.0f}});
    mesh._vertices.push_back(compound::MeshVertex{glm::vec3{0.5f, -0.5f, 0.0f},
                                                  glm::vec3{0.0f, 0.0f, -1.0f},
                                                  glm::vec2{0.0f, 0.0f}});
    mesh._vertices.push_back(compound::MeshVertex{glm::vec3{0.0f, 0.5f, 0.0f},
                                                  glm::vec3{0.0f, 0.0f, -1.0f},
                                                  glm::vec2{0.0f, 0.0f}});
    mesh._indices = {0, 1, 2};
    return mesh;
}

Mesh Mesh::Cube() {
    Mesh mesh;
    mesh._vertices.push_back(
        MeshVertex{{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}});
    mesh._vertices.push_back(
        MeshVertex{{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}});
    mesh._vertices.push_back(
        MeshVertex{{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}});
    mesh._vertices.push_back(
        MeshVertex{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace1 = {0, 1, 3, 1, 2, 3};
    mesh._indices.insert(mesh._indices.end(), indexFace1.begin(),
                         indexFace1.end());

    mesh._vertices.push_back(
        MeshVertex{{-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}});
    mesh._vertices.push_back(
        MeshVertex{{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}});
    mesh._vertices.push_back(
        MeshVertex{{1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}});
    mesh._vertices.push_back(
        MeshVertex{{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace2 = {4, 5, 7, 5, 6, 7};
    mesh._indices.insert(mesh._indices.end(), indexFace2.begin(),
                         indexFace2.end());

    mesh._vertices.push_back(
        MeshVertex{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}});
    mesh._vertices.push_back(
        MeshVertex{{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}});
    mesh._vertices.push_back(
        MeshVertex{{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}});
    mesh._vertices.push_back(
        MeshVertex{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}});
    std::vector<uint32_t> indexFace3 = {8, 9, 11, 9, 10, 11};
    mesh._indices.insert(mesh._indices.end(), indexFace3.begin(),
                         indexFace3.end());

    mesh._vertices.push_back(
        MeshVertex{{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}});
    mesh._vertices.push_back(
        MeshVertex{{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}});
    mesh._vertices.push_back(
        MeshVertex{{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}});
    mesh._vertices.push_back(
        MeshVertex{{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace4 = {12, 13, 15, 13, 14, 15};
    mesh._indices.insert(mesh._indices.end(), indexFace4.begin(),
                         indexFace4.end());

    mesh._vertices.push_back(
        MeshVertex{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}});
    mesh._vertices.push_back(
        MeshVertex{{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}});
    mesh._vertices.push_back(
        MeshVertex{{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}});
    mesh._vertices.push_back(
        MeshVertex{{1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace5 = {16, 17, 19, 17, 18, 19};
    mesh._indices.insert(mesh._indices.end(), indexFace5.begin(),
                         indexFace5.end());

    mesh._vertices.push_back(
        MeshVertex{{-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}});
    mesh._vertices.push_back(
        MeshVertex{{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}});
    mesh._vertices.push_back(
        MeshVertex{{-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}});
    mesh._vertices.push_back(
        MeshVertex{{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}});
    std::vector<uint32_t> indexFace6 = {20, 21, 23, 21, 22, 23};
    mesh._indices.insert(mesh._indices.end(), indexFace6.begin(),
                         indexFace6.end());

    return mesh;
}

Mesh Mesh::Sphere(size_t nRings, size_t nSegments) {
    Mesh mesh;
    for (size_t j = 0; j < nSegments; j++) {
        float theta = 2 * M_PI * float(j + 1) / float(nSegments);
        mesh._vertices.push_back(
            MeshVertex{glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f},
                       glm::vec2{0.0f, theta / (2 * M_PI)}});
    }
    for (size_t i = 0; i < nRings; i++) {
        float phi = M_PI * float(i + 1) / float(nRings);
        for (size_t j = 0; j < nSegments; j++) {
            float theta = 2 * M_PI * float(j + 1) / float(nSegments);
            float x = std::sin(phi) * std::cos(theta);
            float y = std::cos(phi);
            float z = std::sin(phi) * std::sin(theta);
            mesh._vertices.push_back(MeshVertex{
                glm::vec3{x, y, z}, glm::normalize(glm::vec3{x, y, z}),
                glm::vec2{phi / M_PI, theta / (2 * M_PI)}});
        }
        auto v = mesh._vertices[mesh._vertices.size() - 1];
        v.uv.y = 0.0f;
        mesh._vertices.push_back(v);
    }

    for (size_t i = 0; i < nRings + 1; i++) {
        for (size_t j = 0; j < nSegments; j++) {
            size_t k = i * nSegments + j;
            size_t knext = i * nSegments + (j + 1);

            mesh._indices.push_back(k);
            mesh._indices.push_back(knext + nSegments);
            mesh._indices.push_back(k + nSegments);

            mesh._indices.push_back(knext + nSegments);
            mesh._indices.push_back(k);
            mesh._indices.push_back(knext);
        }
    }
    return mesh;
}

Mesh Mesh::Plane(size_t nSub) {
    Mesh mesh;
    glm::vec3 xAxis = {1.0f, 0.0f, 0.0f};
    glm::vec3 yAxis = {0.0f, 1.0f, 0.0f};
    for (size_t i = 0; i < nSub; i++) {
        for (size_t j = 0; j < nSub; j++) {
            float x = ((float)i - ((float)(nSub - 1)) / 2) / float(nSub - 1);
            float y = ((float)j - ((float)(nSub - 1)) / 2) / float(nSub - 1);
            glm::vec3 pos = x * xAxis + y * yAxis;
            mesh._vertices.push_back(MeshVertex(
                pos, glm::vec3{0.0f, 0.0f, 1.0f},
                glm::vec2((float)i / (nSub - 1), (float)j / (nSub - 1))));
        }
    }
    for (size_t i = 0; i < nSub - 1; i++) {
        for (size_t j = 0; j < nSub - 1; j++) {
            mesh._indices.push_back(i * nSub + j);
            mesh._indices.push_back((i + 1) * nSub + j);
            mesh._indices.push_back((i + 1) * nSub + j + 1);

            mesh._indices.push_back((i + 1) * nSub + j + 1);
            mesh._indices.push_back(i * nSub + j + 1);
            mesh._indices.push_back(i * nSub + j);
        }
    }
    return mesh;
}
} // namespace compound