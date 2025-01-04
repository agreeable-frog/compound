#pragma once

#include "log4cplus/log4cplus.h"
#include <GL/glew.h>
#include <GL/gl.h>

namespace compound::impl {
class Texture2D {
public:
    enum class Format { // TODO : move to public (and fuse with other format
                        // enum?)
        RGB,
        RGBA,
        DEPTH,
        DEPTH_STENCIL
    };
    Texture2D(GLsizei width, GLsizei height, Format format, GLsizei levels);
    ~Texture2D();
    void bind();
    bool isBound() const;

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Texture2D.private");
    static GLuint _boundId;
    GLuint _id;
    GLsizei _width;
    GLsizei _height;
    GLsizei _levels;
    Format _format;
    GLenum _GLformat;
    void init();
    void destroy();
};
} // namespace compound::impl