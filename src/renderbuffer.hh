#pragma once

#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GL/gl.h>

namespace compound {
namespace impl {
class RenderBuffer {
public:
    enum class Format { COLOR, DEPTH_STENCIL };
    RenderBuffer(GLsizei width, GLsizei height, Format format);
    void bind();
    bool isBound() const;
    ~RenderBuffer();

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.RenderBuffer.private");
    static GLuint _boundId;
    GLuint _id;
    GLsizei _width;
    GLsizei _height;
    Format _format;
    GLenum _GLformat;
    void init();
    void destroy();
};
} // namespace impl
} // namespace compound