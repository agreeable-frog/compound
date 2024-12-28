#pragma once

#include "log4cplus/log4cplus.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "glm/glm.hpp"

namespace compound {
namespace impl {
class Pipeline {
    friend ::compound::Pipeline;
public:
    Pipeline();
    Pipeline(const Pipeline&);
    Pipeline& operator=(const Pipeline&);
    ~Pipeline();
    void bind();
    void drawElements(size_t count, size_t offset);
    void drawElementsInstanced(size_t count, size_t offset, size_t instanceCount);
    bool isBound() const;
    void setDoCullFace(bool);
    void setDoDepthTest(bool);
    void setClearColor(const glm::vec4&);
private:
    log4cplus::Logger _logger = log4cplus::Logger::getInstance("compound.Pipeline.private");
    GLuint _id;
    static GLuint _boundId;
    bool _doCullFace = true;
    bool _doDepthTest = true;
    glm::vec4 _clearColor = {0.0f, 0.0f, 0.0f, 255.0f};
    void init();
    void destroy();
};
} // namespace impl
} // namespace compound