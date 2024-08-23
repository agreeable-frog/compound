#include "compound/pipeline_state.hh"
#include "pipeline_state.hh"

namespace compound {
namespace impl {
GLuint PipelineState::_boundId = 0;
void PipelineState::init() {
    glGenVertexArrays(1, &_id);
}

void PipelineState::destroy() {
    if (isBound()) _boundId = 0;
    glDeleteVertexArrays(1, &_id);
}

PipelineState::PipelineState() {
    init();
}

PipelineState::PipelineState(const PipelineState& other)
    : _doCullFace(other._doCullFace),
      _doDepthTest(other._doDepthTest),
      _clearColor(other._clearColor) {
    init();
}

PipelineState& PipelineState::operator=(const PipelineState& other) {
    if (this == &other) return *this;
    destroy();
    _doCullFace = other._doCullFace;
    _doDepthTest = other._doDepthTest;
    _clearColor = other._clearColor;
    init();
    return *this;
}

PipelineState::~PipelineState() {
    destroy();
}

void PipelineState::bind() {
    if (!isBound()) {
        LOG4CPLUS_DEBUG(_logger,
                        "Binding vao with id : " + std::to_string(_id));
        glBindVertexArray(_id);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glCullFace(GL_BACK);
        _doCullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
        _doDepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        glClearColor(_clearColor.r, _clearColor.g, _clearColor.b,
                     _clearColor.a);
        _boundId = _id;
    }
}

void PipelineState::TMPdraw(int first, size_t count) {
    glDrawArrays(GL_TRIANGLES, first, count);
}

void PipelineState::setDoCullFace(bool doCullFace) {
    _doCullFace = doCullFace;
}

void PipelineState::setDoDepthTest(bool doDepthTest) {
    _doDepthTest = doDepthTest;
}

void PipelineState::setClearColor(const glm::vec4& color) {
    _clearColor = color;
}

bool PipelineState::isBound() const {
    return _boundId == _id;
}
} // namespace impl

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

void PipelineState::TMPdraw(int first, size_t count) {
    _pImpl->TMPdraw(first, count);
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