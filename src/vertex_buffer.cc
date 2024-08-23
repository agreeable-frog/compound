#include "compound/vertex_buffer.hh"
#include "vertex_buffer.hh"

namespace compound {
namespace impl {

Vertex::BindingDescriptor MeshVertex::getBindingDescriptor() const {
    Vertex::BindingDescriptor bindingDescriptor;
    bindingDescriptor.stride = sizeof(MeshVertex);
    bindingDescriptor.divisor = 0;
    return bindingDescriptor;
}

std::vector<Vertex::AttributeDescriptor> MeshVertex::getAttributeDescriptors()
    const {
    std::vector<Vertex::AttributeDescriptor> attributeDescriptors(
        3, AttributeDescriptor());

    attributeDescriptors[0].location = 0;
    attributeDescriptors[0].size = 3;
    attributeDescriptors[0].type = GL_FLOAT;
    attributeDescriptors[0].normalized = GL_FALSE;
    attributeDescriptors[0].offset = offsetof(MeshVertex, pos);

    attributeDescriptors[1].location = 1;
    attributeDescriptors[1].size = 4;
    attributeDescriptors[1].type = GL_FLOAT;
    attributeDescriptors[1].normalized = GL_FALSE;
    attributeDescriptors[1].offset = offsetof(MeshVertex, color);

    return attributeDescriptors;
}

GLuint VertexBufferBase::_boundId = 0;

template <class T>
void VertexBuffer<T>::init() {
    glGenBuffers(1, &_id);
}

template <class T>
void VertexBuffer<T>::destroy() {
    if (isBound()) _boundId = 0;
    glDeleteBuffers(1, &_id);
}

template <class T>
VertexBuffer<T>::VertexBuffer(::compound::VertexBuffer::Usage usage)
    : std::vector<T>(), _usage(usage) {
    init();
}

template <class T>
VertexBuffer<T>::VertexBuffer(const VertexBuffer<T>& other)
    : std::vector<T>(other), _usage(other._usage) {
    init();
}

template <class T>
VertexBuffer<T>& VertexBuffer<T>::operator=(const VertexBuffer<T>& other) {
    if (this == &other) return *this;
    destroy();
    std::vector<T>::operator=(other);
    _usage = other._usage;
    init();
    return *this;
}

template <class T>
VertexBuffer<T>::~VertexBuffer() {
    destroy();
}

template <class T>
void VertexBuffer<T>::bind() {
    if (!isBound()) {
        LOG4CPLUS_DEBUG(
            _logger, "Binding vertex buffer with id : " + std::to_string(_id));
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        _boundId = _id;
    }
}

template <class T>
bool VertexBuffer<T>::isBound() const {
    return _boundId == _id;
}

template <class T>
void VertexBuffer<T>::attrib() {
    Vertex::BindingDescriptor bindingDescriptor = T().getBindingDescriptor();
    std::vector<Vertex::AttributeDescriptor> attributeDescritors =
        T().getAttributeDescriptors();
    for (const auto& attributeDescriptor : attributeDescritors) {
        glVertexAttribPointer(
            attributeDescriptor.location, attributeDescriptor.size,
            attributeDescriptor.type, attributeDescriptor.normalized,
            bindingDescriptor.stride, (void*)attributeDescriptor.offset);
        glVertexAttribDivisor(attributeDescriptor.location,
                              bindingDescriptor.divisor);
        glEnableVertexAttribArray(attributeDescriptor.location);
    }
}

template <class T>
void VertexBuffer<T>::bufferData() {
    static std::map<::compound::VertexBuffer::Usage, GLenum> usageToGL = {
        {::compound::VertexBuffer::Usage::STATIC, GL_STATIC_DRAW},
        {::compound::VertexBuffer::Usage::STREAM, GL_STREAM_DRAW},
        {::compound::VertexBuffer::Usage::DYNAMIC, GL_DYNAMIC_DRAW}};
    glBufferData(GL_ARRAY_BUFFER, this->size() * sizeof(T), this->data(),
                 usageToGL[_usage]);
}

template <>
void VertexBuffer<MeshVertex>::TMPPushTriangle() {
    push_back(MeshVertex{glm::vec3{-0.5f, -0.5f, 0.0f},
                         glm::vec4{1.0f, 0.0f, 0.0f, 1.0f}});
    push_back(MeshVertex{glm::vec3{0.5f, -0.5f, 0.0f},
                         glm::vec4{1.0f, 0.0f, 0.0f, 1.0f}});
    push_back(MeshVertex{glm::vec3{0.0f, 0.5f, 0.0f},
                         glm::vec4{1.0f, 0.0f, 0.0f, 1.0f}});
}

template class VertexBuffer<MeshVertex>; // explicit template instanciation

} // namespace impl

MeshVertexBuffer::MeshVertexBuffer(VertexBuffer::Usage usage)
    : _pImpl(new impl::VertexBuffer<impl::MeshVertex>(usage)) {
}

MeshVertexBuffer::MeshVertexBuffer(const MeshVertexBuffer& other)
    : _pImpl(new impl::VertexBuffer<impl::MeshVertex>(other._pImpl->_usage)) {
}

MeshVertexBuffer& MeshVertexBuffer::operator=(const MeshVertexBuffer& other) {
    if (this == &other) return *this;
    *_pImpl = *other._pImpl;
    return *this;
}

MeshVertexBuffer::~MeshVertexBuffer() {
}

void MeshVertexBuffer::bind() {
    _pImpl->bind();
}

void MeshVertexBuffer::attrib() {
    _pImpl->attrib();
}

void MeshVertexBuffer::bufferData() {
    _pImpl->bufferData();
}

void MeshVertexBuffer::TMPPushTriangle() {
    _pImpl->TMPPushTriangle();
}
} // namespace compound