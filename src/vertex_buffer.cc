#include "compound/vertex_buffer.hh"
#include "vertex_buffer.hh"

namespace compound {
namespace impl {

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
VertexBuffer<T>::VertexBuffer(::compound::VertexBufferUsage usage)
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
    static std::map<::compound::Vertex::AttributeDescriptor::Type, GLenum>
        typeToGL = {
            {::compound::Vertex::AttributeDescriptor::Type::FLOAT, GL_FLOAT}};
    Vertex::BindingDescriptor bindingDescriptor = T().getBindingDescriptor();
    std::vector<Vertex::AttributeDescriptor> attributeDescritors =
        T().getAttributeDescriptors();
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

template <class T>
void VertexBuffer<T>::bufferData() {
    static std::map<::compound::VertexBufferUsage, GLenum> usageToGL = {
        {::compound::VertexBufferUsage::STATIC, GL_STATIC_DRAW},
        {::compound::VertexBufferUsage::STREAM, GL_STREAM_DRAW},
        {::compound::VertexBufferUsage::DYNAMIC, GL_DYNAMIC_DRAW}};
    glBufferData(GL_ARRAY_BUFFER, this->size() * sizeof(T), this->data(),
                 usageToGL[_usage]);
}

template class VertexBuffer<MeshVertex>;

} // namespace impl

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
    attributeDescriptors[0].type =
        ::compound::Vertex::AttributeDescriptor::Type::FLOAT;
    attributeDescriptors[0].normalized = GL_FALSE;
    attributeDescriptors[0].offset = offsetof(MeshVertex, pos);

    attributeDescriptors[1].location = 1;
    attributeDescriptors[1].size = 4;
    attributeDescriptors[1].type =
        ::compound::Vertex::AttributeDescriptor::Type::FLOAT;
    attributeDescriptors[1].normalized = GL_FALSE;
    attributeDescriptors[1].offset = offsetof(MeshVertex, color);

    return attributeDescriptors;
}

template <class T>
VertexBuffer<T>::VertexBuffer(VertexBufferUsage usage)
    : _pImpl(new impl::VertexBuffer<T>(usage)) {
}

template <class T>
VertexBuffer<T>::VertexBuffer(const VertexBuffer<T>& other)
    : _pImpl(new impl::VertexBuffer<T>(other._pImpl->_usage)) {
}

template <class T>
VertexBuffer<T>& VertexBuffer<T>::operator=(const VertexBuffer<T>& other) {
    if (this == &other) return *this;
    *_pImpl = *other._pImpl;
    return *this;
}

template <class T>
VertexBuffer<T>::~VertexBuffer() {
}

template <class T>
void VertexBuffer<T>::bind() {
    _pImpl->bind();
}

template <class T>
void VertexBuffer<T>::attrib() {
    _pImpl->attrib();
}

template <class T>
void VertexBuffer<T>::bufferData() {
    _pImpl->bufferData();
}

template <class T>
std::vector<T>& VertexBuffer<T>::vector() {
    return dynamic_cast<std::vector<T>&>(*_pImpl);
}

template class VertexBuffer<MeshVertex>;

} // namespace compound