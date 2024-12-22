#include "renderbuffer.hh"

namespace compound {
namespace impl {
GLuint RenderBuffer::_boundId = 0;
RenderBuffer::RenderBuffer(GLsizei width, GLsizei height, Format format)
    : _width(width), _height(height), _format(format) {
    init();
}

RenderBuffer::~RenderBuffer() {
    destroy();
}

void RenderBuffer::bind() {
    if (!isBound()) {
        LOG4CPLUS_DEBUG(_logger, "Binding renderbuffer " + std::to_string(_id));
        glBindRenderbuffer(GL_RENDERBUFFER, _id);
        _boundId = _id;
    }
}

bool RenderBuffer::isBound() const {
    return _boundId == _id;
}

void RenderBuffer::init() {
    glGenRenderbuffers(1, &_id);
    switch (_format) {
        case Format::COLOR:
            _GLformat = GL_RGBA;
            break;
        case Format::DEPTH_STENCIL:
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

void RenderBuffer::destroy() {
    if (isBound()) {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        _boundId = 0;
    }
    glDeleteRenderbuffers(1, &_id);
}
} // namespace impl
} // namespace compound