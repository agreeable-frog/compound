#pragma once

#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include "renderbuffer.hh"

namespace compound {
namespace impl {
class Framebuffer {
    friend ::compound::Framebuffer;

public:
    Framebuffer(GLsizei width, GLsizei height);
    ~Framebuffer();
    void bind();
    static void unbind();
    void bindRead() const;
    static void unbindRead();
    void bindDraw();
    static void unbindDraw();
    void attach(::compound::Framebuffer::Attachment attachment,
                const Renderbuffer& renderbuffer);
    bool isBound() const;
    bool isBoundRead() const;
    bool isBoundDraw() const;
    std::vector<unsigned char> readPixels(
        GLint x, GLint y, GLsizei width, GLsizei height,
        ::compound::Framebuffer::Attachment attachment);

protected:
    static GLuint _boundId;
    static GLuint _boundReadId;
    static GLuint _boundDrawId;
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Framebuffer.private");
    GLuint _id = 0;
    GLsizei _width = 0;
    GLsizei _height = 0;
    void init();
    void destroy();
};
} // namespace impl
} // namespace compound