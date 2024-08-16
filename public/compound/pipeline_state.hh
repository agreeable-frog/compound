#pragma once

#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"

namespace compound {
    namespace impl {
        class PipelineState;
    }
    class PipelineState {
        friend impl::PipelineState;
    public:
        PipelineState();
        PipelineState(const PipelineState&);
        PipelineState& operator=(const PipelineState&);
        ~PipelineState();
        void bind();
        void setDoCullFace(bool);
        void setDoDepthTest(bool);
        void setClearColor(const glm::vec4&);
    private:
        log4cplus::Logger _logger = log4cplus::Logger::getInstance("compound.PipelineState.public");
        std::unique_ptr<impl::PipelineState> _pImpl;
    };
}