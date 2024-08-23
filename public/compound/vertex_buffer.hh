#pragma once

#include <memory>
#include "log4cplus/log4cplus.h"

namespace compound {
namespace impl {
struct MeshVertex;
template <class T>
class VertexBuffer;
} // namespace impl
class VertexBuffer {
public:
    enum class Usage { STATIC, STREAM, DYNAMIC };
    virtual void bind() = 0;
    virtual void attrib() = 0;
    virtual void bufferData() = 0;
};

class MeshVertexBuffer : public VertexBuffer {
    friend impl::VertexBuffer<impl::MeshVertex>;

public:
    MeshVertexBuffer(VertexBuffer::Usage);
    MeshVertexBuffer(const MeshVertexBuffer&);
    MeshVertexBuffer& operator=(const MeshVertexBuffer&);
    ~MeshVertexBuffer();
    void bind() override;
    void attrib() override;
    void bufferData() override;
    void TMPPushTriangle();

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.MeshVertexBuffer.public");
    std::unique_ptr<impl::VertexBuffer<impl::MeshVertex>> _pImpl;
};
} // namespace compound