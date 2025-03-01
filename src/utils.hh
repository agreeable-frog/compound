#include "log4cplus/log4cplus.h"
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GL/gl.h>

inline GLsizei toGLsizei(unsigned int i) {
    if (i > std::numeric_limits<GLsizei>::max()) {
        LOG4CPLUS_WARN(log4cplus::Logger::getInstance("compound"),
                       "Clamping unsigned int to GLuint max");
        return std::numeric_limits<GLuint>::max();
    }
    return i;
}