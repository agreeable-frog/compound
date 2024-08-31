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

void VertexBuffer::attrib(const PipelineState& pipelineState, Vertex::Descriptor descriptor) {
    static std::map<Vertex::AttributeDescriptor::Type, GLenum> typeToGL = {
        {Vertex::AttributeDescriptor::Type::FLOAT, GL_FLOAT}};
    Vertex::BindingDescriptor bindingDescriptor = descriptor.bindingDescriptor;
    std::vector<Vertex::AttributeDescriptor> attributeDescritors =
        descriptor.attributeDescriptors;
    for (const auto& attributeDescriptor : attributeDescritors) {
        glVertexAttribPointer(
            attributeDescriptor.location, attributeDescriptor.size,
            typeToGL[attributeDescriptor.type], attributeDescriptor.normalized,
            bindingDescriptor.stride, (void*)attributeDescriptor.offset);
        glVertexAttribDivisor(attributeDescriptor.location,
                              bindingDescriptor.divisor);
        glEnableVertexAttribArray(attributeDescriptor.location);
    }
}

void VertexBuffer::bufferData(const void* data, size_t size) {
    static std::map<::compound::VertexBuffer::Usage, GLenum> usageToGL = {
        {::compound::VertexBuffer::Usage::STATIC, GL_STATIC_DRAW},
        {::compound::VertexBuffer::Usage::STREAM, GL_STREAM_DRAW},
        {::compound::VertexBuffer::Usage::DYNAMIC, GL_DYNAMIC_DRAW}};
    if (!isBound()) {
        LOG4CPLUS_FATAL(_logger, "Trying to buffer data in a not bound vertex buffer");
        throw std::runtime_error("buffer data in not bound vertex buffer");
    }
    glBufferData(GL_ARRAY_BUFFER, size, data,
                 usageToGL[_usage]);
}
} // namespace impl

const Vertex::Descriptor MeshVertex::getDescriptor() const {
    Descriptor descriptor;
    descriptor.bindingDescriptor.stride = sizeof(MeshVertex);
    descriptor.bindingDescriptor.divisor = 0;

    std::vector<Vertex::AttributeDescriptor> attributeDescriptors(
        3, AttributeDescriptor());

    attributeDescriptors[0].location = 0;
    attributeDescriptors[0].size = 3;
    attributeDescriptors[0].type = Vertex::AttributeDescriptor::Type::FLOAT;
    attributeDescriptors[0].normalized = GL_FALSE;
    attributeDescriptors[0].offset = offsetof(MeshVertex, pos);

    attributeDescriptors[1].location = 1;
    attributeDescriptors[1].size = 4;
    attributeDescriptors[1].type = Vertex::AttributeDescriptor::Type::FLOAT;
    attributeDescriptors[1].normalized = GL_FALSE;
    attributeDescriptors[1].offset = offsetof(MeshVertex, color);

    descriptor.attributeDescriptors = attributeDescriptors;
    return descriptor;
}

size_t MeshVertex::size() const {
    return sizeof(MeshVertex);
}

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

void VertexBuffer::attrib(const PipelineState& pipelineState, Vertex::Descriptor descriptor) {
    _pImpl->attrib(*pipelineState._pImpl, descriptor);
}

void VertexBuffer::bufferData(const void* data, size_t size) {
    _pImpl->bufferData(data, size);
}
} // namespace compound