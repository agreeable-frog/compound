#pragma once

#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>

namespace compound {
namespace impl {

class VertexBufferBase {
protected:
    static GLuint _boundId;
};

template <class T>
class VertexBuffer : public std::vector<T>, public VertexBufferBase {
    friend ::compound::MeshVertexBuffer;

public:
    VertexBuffer(::compound::VertexBufferUsage usage);
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
    ::compound::VertexBufferUsage _usage;
    void init();
    void destroy();
};
} // namespace impl
} // namespace compound