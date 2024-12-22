#pragma once

#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GL/gl.h>

namespace compound {
namespace impl {
class Framebuffer {
public:
    Framebuffer(GLsizei width, GLsizei height);
    ~Framebuffer();
    void bind();
    bool isBound() const;

private:
    static GLuint _boundId;
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Framebuffer.private");
    GLuint _id = 0;
    GLsizei _width = 0;
    GLsizei _height = 0;
    GLenum _format = 0;
    void init();
    void destroy();
};
} // namespace impl
} // namespace compound