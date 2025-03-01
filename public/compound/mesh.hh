#pragma once

#include "log4cplus/log4cplus.h"
#include "vertex_buffer.hh"
#include "index_buffer.hh"
#include <memory>

namespace compound {

struct alignas(16) MeshVertex : public Vertex {
public:
    MeshVertex() {
    }
    MeshVertex(const glm::vec3& pos, const glm::vec3& normal,
               const glm::vec2& uv)
        : pos(pos), normal(normal), uv(uv) {
    }
    const Descriptor getDescriptor() const override;
    size_t size() const override;
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh {
public:
    Mesh();
    static void loadMeshes(std::vector<std::shared_ptr<Mesh>> meshes,
                           std::shared_ptr<VertexBuffer> pVertexBuffer,
                           std::shared_ptr<IndexBuffer> pIndexBuffer);
    static Mesh Triangle();
    static Mesh Cube();
    static Mesh Sphere(size_t nRings, size_t nSegments);
    static Mesh Plane(size_t nSub);
    size_t nbIndices() const {
        return _indices.size();
    }
    size_t indexBufferOffset() const {
        return _indexBufferOffset;
    }

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Mesh.public");
    std::vector<MeshVertex> _vertices;
    std::weak_ptr<VertexBuffer> _pVertexBuffer;
    std::vector<uint32_t> _indices;
    std::weak_ptr<IndexBuffer> _pIndexBuffer;
    size_t _indexBufferOffset; // location of this mesh in the index buffer
};
} // namespace compound