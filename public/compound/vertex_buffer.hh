#pragma once

#include "compound/pipeline.hh"
#include <memory>
#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"

namespace compound {
namespace impl {
class VertexBuffer;
} // namespace impl
struct Vertex {
    struct BindingDescriptor {
        uint stride = 0;
        uint divisor = 0;
    };
    struct AttributeDescriptor {
        uint location = 0;
        uint size = 0;
        enum class Type { FLOAT };
        Type type = Type::FLOAT;
        bool normalized = false;
        size_t offset = 0;
    };
    struct Descriptor {
        BindingDescriptor bindingDescriptor;
        std::vector<AttributeDescriptor> attributeDescriptors;
    };
    virtual const Descriptor getDescriptor() const = 0;
    virtual size_t size() const = 0;
};

class VertexBuffer {
public:
    enum class Usage { STATIC, STREAM, DYNAMIC };
    VertexBuffer(Usage);
    VertexBuffer(const VertexBuffer&);
    VertexBuffer& operator=(const VertexBuffer&);
    ~VertexBuffer();
    void bind();
    void attrib(const Pipeline&, Vertex::Descriptor);
    void bufferData(const void* data, size_t size,
                    Vertex::Descriptor descriptor);

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.VertexBuffer.public");
    std::unique_ptr<impl::VertexBuffer> _pImpl;
};

} // namespace compound