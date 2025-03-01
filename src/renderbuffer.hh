#pragma once

#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include "compound/framebuffer.hh"

namespace compound {
namespace impl {
class Renderbuffer {
public:
    Renderbuffer(GLsizei width, GLsizei height,
                 ::compound::Renderbuffer::Format format);
    void bind();
    GLuint id() const;
    bool isBound() const;
    ~Renderbuffer();

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Renderbuffer.private");
    static GLuint _boundId;
    GLuint _id;
    GLsizei _width;
    GLsizei _height;
    ::compound::Renderbuffer::Format _format;
    GLenum _GLformat;
    void init();
    void destroy();
};
} // namespace impl
} // namespace compound