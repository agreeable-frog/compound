#pragma once

#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GL/gl.h>

namespace compound {
namespace impl {
class Framebuffer {
//    friend ::compound::Framebuffer;
public:
    Framebuffer(size_t width, size_t height, size_t depth);

private:
    GLuint _id;
    size_t _width;
    size_t _height;
    size_t _depth;
    GLenum _format;
};
}
}