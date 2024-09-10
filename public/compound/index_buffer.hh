#pragma once

#include <vector>
#include <memory>
#include "log4cplus/log4cplus.h"

namespace compound {
namespace impl {
class IndexBuffer;
}

class IndexBuffer {
public:
    enum class Usage { STATIC, STREAM, DYNAMIC };
    IndexBuffer(Usage);
    ~IndexBuffer();
    void bind();
    void bufferData(const std::vector<uint32_t>&);

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.IndexBuffer.public");
    std::unique_ptr<impl::IndexBuffer> _pImpl;
};
} // namespace compound