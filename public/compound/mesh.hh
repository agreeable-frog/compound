#pragma once

#include "log4cplus/log4cplus.h"
#include "vertex_buffer.hh"

namespace compound {

struct alignas(16) MeshVertex : public Vertex {
public:
    MeshVertex() {
    }
    MeshVertex(const glm::vec3& pos, const glm::vec4& color)
        : pos(pos), color(color) {
    }
    const Descriptor getDescriptor() const override;
    size_t size() const override;
    glm::vec3 pos;
    glm::vec4 color;
};

class Mesh {
public:
    Mesh();

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Mesh.public");
    std::vector<MeshVertex> _vertices;
    std::vector<uint32_t> _indices;
};
} // namespace compound