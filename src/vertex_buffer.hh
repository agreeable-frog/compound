#pragma once

#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>

namespace compound {
namespace impl {

struct Vertex {
    struct BindingDescriptor {
        GLsizei stride;
        GLuint divisor;
    };

    struct AttributeDescriptor {
        GLuint location;
        GLint size;
        GLenum type;
        GLboolean normalized;
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

class VertexBufferBase {
protected:
    static GLuint _boundId;
};

template <class T>
class VertexBuffer : public std::vector<T>, public VertexBufferBase {
    friend ::compound::MeshVertexBuffer;

public:
    VertexBuffer(::compound::VertexBuffer::Usage usage);
    VertexBuffer(const VertexBuffer&);
    VertexBuffer& operator=(const VertexBuffer&);
    ~VertexBuffer();
    void bind();
    bool isBound() const;
    void attrib();
    void bufferData();
    void TMPPushTriangle();

protected:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.VertexBuffer.private");
    GLuint _id;
    ::compound::VertexBuffer::Usage _usage;
    void init();
    void destroy();
};
} // namespace impl
} // namespace compound