#pragma once

#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"

namespace compound {
        class VertexBuffer;
    namespace impl {
        class Pipeline;
    }
    class Pipeline {
        friend impl::Pipeline;
        friend VertexBuffer;
    public:
        Pipeline();
        Pipeline(const Pipeline&);
        Pipeline& operator=(const Pipeline&);
        ~Pipeline();
        void bind();
        void drawElements(size_t count, size_t offset);
        void drawElementsInstanced(size_t count, size_t offset, size_t instanceCount);
        void setDoCullFace(bool);
        void setDoDepthTest(bool);
        void setClearColor(const glm::vec4&);
    private:
        log4cplus::Logger _logger = log4cplus::Logger::getInstance("compound.Pipeline.public");
        std::unique_ptr<impl::Pipeline> _pImpl;
    };
}