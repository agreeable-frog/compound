#include "renderbuffer.hh"
#include "compound/framebuffer.hh"
#include "utils.hh"

namespace compound {
Renderbuffer::Renderbuffer(unsigned int width, unsigned int height,
                           Format format)
    : _pImpl(
          new impl::Renderbuffer(toGLsizei(width), toGLsizei(height), format)) {
}

void Renderbuffer::bind() {
    _pImpl->bind();
}

Renderbuffer::~Renderbuffer() {
}

namespace impl {
GLuint Renderbuffer::_boundId = 0;
Renderbuffer::Renderbuffer(GLsizei width, GLsizei height,
                           ::compound::Renderbuffer::Format format)
    : _width(width), _height(height), _format(format) {
    init();
}

Renderbuffer::~Renderbuffer() {
    destroy();
}

void Renderbuffer::bind() {
    if (!isBound()) {
        LOG4CPLUS_DEBUG(_logger, "Binding renderbuffer " + std::to_string(_id));
        glBindRenderbuffer(GL_RENDERBUFFER, _id);
        _boundId = _id;
    }
}

GLuint Renderbuffer::id() const {
    return _id;
}

bool Renderbuffer::isBound() const {
    return _boundId == _id;
}

void Renderbuffer::init() {
    glGenRenderbuffers(1, &_id);
    switch (_format) {
        case ::compound::Renderbuffer::Format::COLOR:
            _GLformat = GL_RGBA;
            break;
        case ::compound::Renderbuffer::Format::DEPTH:
            _GLformat = GL_DEPTH_COMPONENT32;
            break;
        case ::compound::Renderbuffer::Format::DEPTH_STENCIL:
            _GLformat = GL_DEPTH24_STENCIL8;
            break;
        default:
            LOG4CPLUS_FATAL(_logger, "Unsupported format");
            throw std::runtime_error("Unsupported format");
    }
    auto oldId = _boundId;
    LOG4CPLUS_DEBUG(_logger, "Binding constructed renderbuffer " +
                                 std::to_string(_id) + " temporarily");
    glBindRenderbuffer(GL_RENDERBUFFER, _id);
    glRenderbufferStorage(GL_RENDERBUFFER, _GLformat, _width, _height);
    glBindRenderbuffer(GL_RENDERBUFFER, oldId);
}

void Renderbuffer::destroy() {
    if (isBound()) {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        _boundId = 0;
    }
    glDeleteRenderbuffers(1, &_id);
}
} // namespace impl
} // namespace compound