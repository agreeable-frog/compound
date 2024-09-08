#pragma once

#include "pipeline.hh"
#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>

namespace compound {
namespace impl {

class VertexBuffer {
    friend ::compound::VertexBuffer;

public:
    VertexBuffer(::compound::VertexBuffer::Usage);
    VertexBuffer(const VertexBuffer&);
    VertexBuffer& operator=(const VertexBuffer&);
    ~VertexBuffer();
    void bind();
    bool isBound() const;
    void attrib(const Pipeline&, ::compound::Vertex::Descriptor);
    void bufferData(const void*, size_t, ::compound::Vertex::Descriptor);

protected:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.VertexBuffer.private");
    GLuint _id;
    static GLuint _boundId;
    ::compound::VertexBuffer::Usage _usage;
    ::compound::Vertex::Descriptor _descriptor;
    void init();
    void destroy();
};
} // namespace impl
} // namespace compound