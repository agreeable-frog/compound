#include "texture_2D_array.hh"

namespace compound {
namespace impl {
GLuint Texture2DArray::_boundId = 0;
Texture2DArray::Texture2DArray() {
    glGenTextures(1, &_id);
}
Texture2DArray::~Texture2DArray() {
    if (isBound()) _boundId = 0;
    glDeleteTextures(1, &_id);
}
void Texture2DArray::bind() {
    if (isBound()) return;
    glBindTexture(GL_TEXTURE_2D_ARRAY, _id);
    _boundId = _id;
}
bool Texture2DArray::isBound() const {
    return _boundId == _id;
}

void Texture2DArray::texStorage3D(size_t levels, size_t width, size_t height,
                                  size_t depth) {
    if (!isBound()) {
        LOG4CPLUS_FATAL(_logger, "Not bound");
        throw std::runtime_error("Not bound");
    }
    _levels = levels;
    _width = width;
    _height = height;
    _depth = depth;
    if (levels == 0 || width == 0 || height == 0 || depth == 0) {
        LOG4CPLUS_FATAL(_logger,
                        "One of the paramaters is equal to 0, what are you "
                        "trying to do here?");
        throw std::runtime_error(
            "One of the paramaters is equal to 0, what are you trying to do "
            "here?");
    }
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, _levels, GL_RGBA8, _width, _height,
                   _depth);
}

void Texture2DArray::texSubImage3D(size_t level, size_t xoffset, size_t yoffset,
                                   size_t zoffset, size_t width, size_t height,
                                   size_t depth, void* data) {
    if (!isBound()) {
        LOG4CPLUS_FATAL(_logger, "Not bound");
        throw std::runtime_error("Not bound");
    }
    if (level >= _levels || xoffset >= _width || yoffset >= _height ||
        zoffset >= _depth || width + xoffset > _width ||
        height + yoffset > _height || depth + zoffset > _depth) {
        // TODO : add trace logs
        LOG4CPLUS_FATAL(_logger, "Invalid parameters for texSubImage3D");
        throw std::runtime_error("Invalid parameters for texSubImage3D");
    }
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, xoffset, yoffset, zoffset,
                    width, height, depth, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture2DArray::generateMipmap() {
    if (!isBound()) {
        LOG4CPLUS_FATAL(_logger, "Not bound");
        throw std::runtime_error("Not bound");
    }
    // TODO : have texparams be configurable
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}
} // namespace impl
} // namespace compound