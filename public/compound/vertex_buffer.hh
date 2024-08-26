#pragma once

#include <memory>
#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"

namespace compound {
namespace impl {
template <class T>
class VertexBuffer;
} // namespace impl
struct Vertex {
    struct BindingDescriptor {
        uint stride;
        uint divisor;
    };

    struct AttributeDescriptor {
        uint location;
        uint size;
        enum class Type { FLOAT };
        Type type;
        bool normalized;
        size_t offset;
    };
    virtual BindingDescriptor getBindingDescriptor() const = 0;
    virtual std::vector<AttributeDescriptor> getAttributeDescriptors()
        const = 0;
};

struct alignas(16) MeshVertex : public Vertex {
public:
    MeshVertex() {
    }
    MeshVertex(const glm::vec3& pos, const glm::vec4& color)
        : pos(pos), color(color) {
    }
    BindingDescriptor getBindingDescriptor() const override;
    std::vector<AttributeDescriptor> getAttributeDescriptors() const override;
    glm::vec3 pos;
    glm::vec4 color;
};

enum class VertexBufferUsage { STATIC, STREAM, DYNAMIC };

template <class T>
class VertexBuffer {
public:
    VertexBuffer(VertexBufferUsage usage);
    VertexBuffer(const VertexBuffer&);
    VertexBuffer& operator=(const VertexBuffer&);
    ~VertexBuffer();
    void bind();
    void attrib();
    void bufferData();
    std::vector<T>& vector();

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.MeshVertexBuffer.public");
    std::unique_ptr<impl::VertexBuffer<T>> _pImpl;
};

typedef VertexBuffer<MeshVertex> MeshVertexBuffer;

} // namespace compound