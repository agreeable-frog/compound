#include "compound/vertex_buffer.hh"
#include "vertex_buffer.hh"

namespace compound {
namespace impl {

GLuint VertexBuffer::_boundId = 0;

void VertexBuffer::init() {
    glGenBuffers(1, &_id);
}

void VertexBuffer::destroy() {
    if (isBound()) _boundId = 0;
    glDeleteBuffers(1, &_id);
}

VertexBuffer::VertexBuffer(::compound::VertexBuffer::Usage usage)
    : _usage(usage) {
    init();
}

VertexBuffer::VertexBuffer(const VertexBuffer& other)
    : _usage(other._usage), _descriptor(other._descriptor) {
    init();
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other) {
    if (this == &other) return *this;
    destroy();
    _usage = other._usage;
    _descriptor = other._descriptor;
    init();
    return *this;
}

VertexBuffer::~VertexBuffer() {
    destroy();
}

void VertexBuffer::bind() {
    if (!isBound()) {
        LOG4CPLUS_DEBUG(
            _logger, "Binding vertex buffer with id : " + std::to_string(_id));
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        _boundId = _id;
    }
}

bool VertexBuffer::isBound() const {
    return _boundId == _id;
}

void VertexBuffer::attrib(const Pipeline& pipelineState,
                          Vertex::Descriptor descriptor) {
    if (!isBound()) {
        LOG4CPLUS_FATAL(_logger, "Trying to attrib not bound vertex buffer");
        throw std::runtime_error("Trying to attrib not bound vertex buffer");
    }
    if (!pipelineState.isBound()) {
        LOG4CPLUS_FATAL(_logger, "Attrib vertex buffer on not bound vao");
        throw std::runtime_error("Attrib vertex buffer on not bound vao");
    }
    static std::map<Vertex::AttributeDescriptor::Type, GLenum> typeToGL = {
        {Vertex::AttributeDescriptor::Type::FLOAT, GL_FLOAT}};
    _descriptor = descriptor;
    Vertex::BindingDescriptor bindingDescriptor = descriptor.bindingDescriptor;
    std::vector<Vertex::AttributeDescriptor> attributeDescritors =
        descriptor.attributeDescriptors;
    for (const auto& attributeDescriptor : attributeDescritors) {
        glVertexAttribPointer(
            attributeDescriptor.location, attributeDescriptor.size,
            typeToGL[attributeDescriptor.type],
            attributeDescriptor.normalized ? GL_TRUE : GL_FALSE,
            bindingDescriptor.stride, (void*)attributeDescriptor.offset);
        glVertexAttribDivisor(attributeDescriptor.location,
                              bindingDescriptor.divisor);
        glEnableVertexAttribArray(attributeDescriptor.location);
    }
}

void VertexBuffer::bufferData(const void* data, size_t size,
                              ::compound::Vertex::Descriptor descriptor) {
    // TODO : compare descriptor with _descriptor
    static std::map<::compound::VertexBuffer::Usage, GLenum> usageToGL = {
        {::compound::VertexBuffer::Usage::STATIC, GL_STATIC_DRAW},
        {::compound::VertexBuffer::Usage::STREAM, GL_STREAM_DRAW},
        {::compound::VertexBuffer::Usage::DYNAMIC, GL_DYNAMIC_DRAW}};
    if (!isBound()) {
        LOG4CPLUS_FATAL(_logger,
                        "Trying to buffer data in a not bound vertex buffer");
        throw std::runtime_error("buffer data in a not bound vertex buffer");
    }
    glBufferData(GL_ARRAY_BUFFER, size, data, usageToGL[_usage]);
}
} // namespace impl

VertexBuffer::VertexBuffer(VertexBuffer::Usage usage)
    : _pImpl(new impl::VertexBuffer(usage)) {
}

VertexBuffer::VertexBuffer(const VertexBuffer& other)
    : _pImpl(new impl::VertexBuffer(other._pImpl->_usage)) {
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other) {
    if (this == &other) return *this;
    *_pImpl = *other._pImpl;
    return *this;
}

VertexBuffer::~VertexBuffer() {
}

void VertexBuffer::bind() {
    _pImpl->bind();
}

void VertexBuffer::attrib(const Pipeline& pipelineState,
                          Vertex::Descriptor descriptor) {
    _pImpl->attrib(*pipelineState._pImpl, descriptor);
}

void VertexBuffer::bufferData(const void* data, size_t size,
                              Vertex::Descriptor descriptor) {
    _pImpl->bufferData(data, size, descriptor);
}
} // namespace compound