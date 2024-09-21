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
    std::weak_ptr<VertexBuffer> _pVertexBuffer;
    std::vector<uint32_t> _indices;
    std::weak_ptr<IndexBuffer> _pIndexBuffer;
    size_t _infexBufferOffset; // location in the indexbuffer where our indices
                               // start, should be multiplied by
                               // sizeof(uint32_t) when used
    size_t _indicesOffset; // value by which our indices where offsetted before
                           // putting them in the index buffer. we could instead
                           // use draw*BaseVertex with this value and not modify
                           // the index values before putting them in the
                           // indexbuffer but there isn't really a cost...
};
} // namespace compound