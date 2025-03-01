#include "texture_2D.hh"

namespace compound {
namespace impl {
GLuint Texture2D::_boundId = 0;
Texture2D::Texture2D(GLsizei width, GLsizei height, Format format,
                     GLsizei levels)
    : _width(width), _height(height), _levels(levels), _format(format) {
}

void Texture2D::init() {
    glGenTextures(1, &_id);
    LOG4CPLUS_DEBUG(_logger, std::string("Temporarily binding ") +
                                 std::to_string(_id) + " to GL_TEXTURE_2D");
    glBindTexture(GL_TEXTURE_2D, _id);
    switch (_format) {
        case Format::RGB:
            _GLformat = GL_RGB8;
            break;
        case Format::RGBA:
            _GLformat = GL_RGBA8;
            break;
        case Format::DEPTH:
            _GLformat = GL_DEPTH_COMPONENT24;
            break;
        case Format::DEPTH_STENCIL:
            _GLformat = GL_DEPTH24_STENCIL8;
            break;
    }
    glTextureStorage2D(GL_TEXTURE_2D, _levels, _GLformat, _width, _height);
    glBindTexture(GL_TEXTURE_2D, _boundId);
}

void Texture2D::bind() {
    if (!isBound()) {
        glBindTexture(GL_TEXTURE_2D, _id);
        _boundId = _id;
    }
}

bool Texture2D::isBound() const {
    return _id == _boundId;
}

void Texture2D::destroy() {
    if (_id != 0) {
        if (isBound()) {
            _boundId = 0;
        }
        glDeleteTextures(1, &_id);
    }
}

Texture2D::~Texture2D() {
    destroy();
}
} // namespace impl
} // namespace compound