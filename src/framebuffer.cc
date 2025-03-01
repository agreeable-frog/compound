#include "framebuffer.hh"
#include "compound/framebuffer.hh"
#include "utils.hh"

#include <iostream>

namespace compound {

Framebuffer::Framebuffer(unsigned int width, unsigned int height)
    : _pImpl(new impl::Framebuffer(toGLsizei(width), toGLsizei(height))) {
}

void Framebuffer::bind() {
    _pImpl->bind();
}

void Framebuffer::unbind() {
    impl::Framebuffer::unbind();
}

void Framebuffer::bindRead() {
    _pImpl->bindRead();
}

void Framebuffer::unbindRead() {
    impl::Framebuffer::unbindRead();
}

void Framebuffer::bindDraw() {
    _pImpl->bindDraw();
}

void Framebuffer::unbindDraw() {
    impl::Framebuffer::unbindDraw();
}

void Framebuffer::attach(Attachment attachment,
                         const Renderbuffer& renderbuffer) {
    _pImpl->attach(attachment, *renderbuffer._pImpl);
}

std::vector<unsigned char> Framebuffer::readPixels(
    int x, int y, size_t width, size_t height,
    Framebuffer::Attachment attachment) {
    return _pImpl->readPixels((GLint)x, (GLint)y, toGLsizei(width), toGLsizei(height),
                       attachment);
}

Framebuffer::~Framebuffer() {
}

namespace impl {
GLuint Framebuffer::_boundId = 0;
GLuint Framebuffer::_boundReadId = 0;
GLuint Framebuffer::_boundDrawId = 0;

Framebuffer::Framebuffer(GLsizei width, GLsizei height)
    : _width(width), _height(height) {
    init();
}

Framebuffer::~Framebuffer() {
    destroy();
}

void Framebuffer::bind() {
    if (!isBound()) {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        _boundId = _id;
    }
}

void Framebuffer::unbind() {
    if (_boundId != 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        _boundId = 0;
    }
}

void Framebuffer::bindRead() const {
    if (!isBoundRead()) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, _id);
        _boundReadId = _id;
    }
}

void Framebuffer::unbindRead() {
    if (_boundReadId != 0) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        _boundReadId = 0;
    }
}

void Framebuffer::unbindDraw() {
    if (_boundDrawId != 0) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        _boundDrawId = 0;
    }
}

void Framebuffer::bindDraw() {
    if (!isBoundDraw()) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _id);
        _boundDrawId = _id;
    }
}

void Framebuffer::attach(::compound::Framebuffer::Attachment attachment,
                         const Renderbuffer& renderbuffer) {
    if (!isBound()) {
        LOG4CPLUS_ERROR(_logger, "Not bound");
        return;
    }
    GLenum glAttachment;
    switch (attachment) {
        case ::compound::Framebuffer::Attachment::COLOR_ATTACHMENT0:
            glAttachment = GL_COLOR_ATTACHMENT0;
            break;
        case ::compound::Framebuffer::Attachment::DEPTH:
            glAttachment = GL_DEPTH_ATTACHMENT;
            break;
        case ::compound::Framebuffer::Attachment::DEPTH_STENCIL:
            glAttachment = GL_DEPTH_STENCIL_ATTACHMENT;
            break;
        default:
            LOG4CPLUS_ERROR(_logger, "Unrecognized attachment");
            return;
    }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, glAttachment, GL_RENDERBUFFER,
                              renderbuffer.id());
}

std::vector<unsigned char> Framebuffer::readPixels(
    GLint x, GLint y, GLsizei width, GLsizei height,
    ::compound::Framebuffer::Attachment attachment) {
    // TODO : check framebuffer is bound
    size_t bufferSize = width * height;
    std::vector<unsigned char> data;
    switch (attachment) {
        case ::compound::Framebuffer::Attachment::COLOR_ATTACHMENT0: {
            bufferSize *= 4;
            data.reserve(bufferSize);
            glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
                         data.data());
            break;
        }
        default: {
            LOG4CPLUS_WARN(_logger, "not implemented");
        }
    }
    return data;
}

bool Framebuffer::isBound() const {
    return _id == _boundId;
}

bool Framebuffer::isBoundRead() const {
    return _id == _boundReadId;
}

bool Framebuffer::isBoundDraw() const {
    return _id == _boundDrawId;
}

void Framebuffer::init() {
    glGenFramebuffers(1, &_id);
    LOG4CPLUS_TRACE(_logger,
                    "Creating framebuffer with id : " + std::to_string(_id));
}

void Framebuffer::destroy() {
    if (isBound()) _boundId = 0;
    if (isBoundRead()) _boundReadId = 0;
    if (isBoundDraw()) _boundDrawId = 0;
    glDeleteFramebuffers(1, &_id);
}
} // namespace impl
} // namespace compound