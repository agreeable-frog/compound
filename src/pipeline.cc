#include "compound/pipeline.hh"
#include "pipeline.hh"

namespace compound {
namespace impl {
GLuint Pipeline::_boundId = 0;
void Pipeline::init() {
    glGenVertexArrays(1, &_id);
}

void Pipeline::destroy() {
    if (isBound()) _boundId = 0;
    glDeleteVertexArrays(1, &_id);
}

Pipeline::Pipeline() {
    init();
}

Pipeline::Pipeline(const Pipeline& other)
    : _doCullFace(other._doCullFace),
      _doDepthTest(other._doDepthTest),
      _clearColor(other._clearColor) {
    init();
}

Pipeline& Pipeline::operator=(const Pipeline& other) {
    if (this == &other) return *this;
    destroy();
    _doCullFace = other._doCullFace;
    _doDepthTest = other._doDepthTest;
    _clearColor = other._clearColor;
    init();
    return *this;
}

Pipeline::~Pipeline() {
    destroy();
}

void Pipeline::bind() {
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

void Pipeline::drawElements(size_t count, size_t offset) {
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT,
                   (void*)(offset * sizeof(uint32_t)));
}

void Pipeline::drawElementsInstanced(size_t count, size_t offset,
                                     size_t instanceCount) {
    glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT,
                            (void*)(offset * sizeof(uint32_t)), instanceCount);
}

void Pipeline::setDoCullFace(bool doCullFace) {
    _doCullFace = doCullFace;
}

void Pipeline::setDoDepthTest(bool doDepthTest) {
    _doDepthTest = doDepthTest;
}

void Pipeline::setClearColor(const glm::vec4& color) {
    _clearColor = color;
}

bool Pipeline::isBound() const {
    return _boundId == _id;
}
} // namespace impl

Pipeline::Pipeline() : _pImpl(new impl::Pipeline()) {
}

Pipeline::Pipeline(const Pipeline& other)
    : _pImpl(new impl::Pipeline(*other._pImpl)) {
}

Pipeline& Pipeline::operator=(const Pipeline& other) {
    if (this == &other) return *this;
    *_pImpl = *other._pImpl;
    return *this;
}

Pipeline::~Pipeline() {
}

void Pipeline::bind() {
    _pImpl->bind();
}

void Pipeline::drawElements(size_t count, size_t offset) {
    _pImpl->drawElements(count, offset);
}

void Pipeline::drawElementsInstanced(size_t count, size_t offset,
                                     size_t instanceCount) {
    _pImpl->drawElementsInstanced(count, offset, instanceCount);
}

void Pipeline::setDoCullFace(bool doCullFace) {
    _pImpl->setDoCullFace(doCullFace);
}

void Pipeline::setDoDepthTest(bool doDepthTest) {
    _pImpl->setDoDepthTest(doDepthTest);
}

void Pipeline::setClearColor(const glm::vec4& color) {
    _pImpl->setClearColor(color);
}
} // namespace compound