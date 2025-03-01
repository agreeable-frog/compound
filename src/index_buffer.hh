#pragma once

#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GL/gl.h>

namespace compound {
namespace impl {
class IndexBuffer {
    friend ::compound::IndexBuffer;

public:
    IndexBuffer(::compound::IndexBuffer::Usage);
    ~IndexBuffer();
    void bind();
    bool isBound() const;
    void bufferData(const std::vector<uint32_t>&);

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.IndexBuffer.private");
    static GLuint _boundId;
    GLuint _id;
    ::compound::IndexBuffer::Usage _usage;
    void init();
    void destroy();
};
} // namespace impl

} // namespace compound