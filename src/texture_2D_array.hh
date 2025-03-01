#pragma once

#include "log4cplus/log4cplus.h"
#include <GL/glew.h>
#include <GL/gl.h>

namespace compound {
namespace impl {
// TODO : make top Texture parent class
class Texture2DArray {
public:
    Texture2DArray();
    ~Texture2DArray();
    void bind();
    bool isBound() const;
    void texStorage3D(size_t levels, size_t width, size_t height, size_t depth);
    void texSubImage3D(size_t level, size_t xoffset, size_t yoffset,
                       size_t zoffset, size_t width, size_t height,
                       size_t depth, void* data);
    void generateMipmap();

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Texture2DArray.private");
    GLuint _id = 0;
    static GLuint _boundId;
    size_t _levels = 0;
    size_t _width = 0;
    size_t _height = 0;
    size_t _depth = 0;
};
} // namespace impl
} // namespace compound