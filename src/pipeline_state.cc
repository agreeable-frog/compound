#include "compound/pipeline_state.hh"
#include "pipeline_state.hh"

namespace compound {
namespace impl {}

PipelineState::PipelineState() : _pImpl(new impl::PipelineState()) {
}

PipelineState::PipelineState(const PipelineState& other)
    : _pImpl(new impl::PipelineState(*other._pImpl)) {
}

PipelineState& PipelineState::operator=(const PipelineState& other) {
    if (this == &other) return *this;
    *_pImpl = *other._pImpl;
    return *this;
}

PipelineState::~PipelineState() {
}

void PipelineState::bind() {
    _pImpl->bind();
}

void PipelineState::setDoCullFace(bool doCullFace) {
    _pImpl->setDoCullFace(doCullFace);
}

void PipelineState::setDoDepthTest(bool doDepthTest) {
    _pImpl->setDoDepthTest(doDepthTest);
}

void PipelineState::setClearColor(const glm::vec4& color) {
    _pImpl->setClearColor(color);
}
} // namespace compound