#pragma once

#include "log4cplus/log4cplus.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "glm/glm.hpp"

namespace compound {
namespace impl {
class PipelineState {
    friend ::compound::PipelineState;
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
    log4cplus::Logger _logger = log4cplus::Logger::getInstance("compound.PipelineState.private");
    GLuint _id;
    bool _doCullFace = true;
    bool _doDepthTest = true;
    glm::vec4 _clearColor = {0.0f, 0.0f, 0.0f, 0.0f};
    void init();
};
} // namespace impl
} // namespace compound