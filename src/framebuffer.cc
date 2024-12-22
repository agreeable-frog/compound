#include "framebuffer.hh"

namespace compound {
namespace impl {
GLuint Framebuffer::_boundId = 0;

Framebuffer::Framebuffer(GLsizei width, GLsizei height)
    : _width(width), _height(height) {
    init();
}

void Framebuffer::bind() {
    if (!isBound()) {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        _boundId = _id;
    }
}

bool Framebuffer::isBound() const {
    return _id == _boundId;
}

void Framebuffer::init() {
    glGenFramebuffers(1, &_id);
}

void Framebuffer::destroy() {
    if (isBound()) _boundId = 0;
    glDeleteFramebuffers(1, &_id);
}
} // namespace impl
} // namespace compound