#include "compound/index_buffer.hh"
#include "index_buffer.hh"

namespace compound {
namespace impl {

GLuint IndexBuffer::_boundId = 0;

void IndexBuffer::init() {
    glGenBuffers(1, &_id);
}

void IndexBuffer::destroy() {
    if (isBound()) _boundId = 0;
    glDeleteBuffers(1, &_id);
}

IndexBuffer::IndexBuffer(::compound::IndexBuffer::Usage usage) : _usage(usage) {
    init();
}

IndexBuffer::~IndexBuffer() {
    destroy();
}

void IndexBuffer::bind() {
    if (!isBound()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
        _boundId = _id;
    }
}

bool IndexBuffer::isBound() const {
    return _boundId == _id;
}

void IndexBuffer::bufferData(const std::vector<uint32_t>& data) {
    static std::map<::compound::IndexBuffer::Usage, GLenum> usageToGL = {
        {::compound::IndexBuffer::Usage::STATIC, GL_STATIC_DRAW},
        {::compound::IndexBuffer::Usage::STREAM, GL_STREAM_DRAW},
        {::compound::IndexBuffer::Usage::DYNAMIC, GL_DYNAMIC_DRAW}};
    if (!isBound()) {
        LOG4CPLUS_FATAL(_logger,
                        "Trying to buffer data in a not bound index buffer");
        throw std::runtime_error("Trying to buffer data in a not bound index buffer");
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(uint32_t), data.data(),
                 usageToGL[_usage]);
}
} // namespace impl
IndexBuffer::IndexBuffer(IndexBuffer::Usage usage)
    : _pImpl(new impl::IndexBuffer(usage)) {
}

IndexBuffer::~IndexBuffer() {
}

void IndexBuffer::bind() {
    _pImpl->bind();
}

void IndexBuffer::bufferData(const std::vector<uint32_t>& data) {
    _pImpl->bufferData(data);
}
} // namespace compound