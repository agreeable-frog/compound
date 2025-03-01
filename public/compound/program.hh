#pragma once

#include "log4cplus/log4cplus.h"
#include <string>
#include <memory>
#include "glm/glm.hpp"

namespace compound {
namespace impl {
class Program;
}
class Program {
public:
    Program(const std::string& vertShaderPath,
            const std::string& fragShaderPath);
    Program(const Program&);
    Program& operator=(const Program&);
    ~Program();
    void bind();
    void setUniform(int location, const glm::mat4& data);
    void setUniform(int location, const glm::vec4& data);
    void setUniform(int location, float data);

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Program.public");
    std::unique_ptr<impl::Program> _pImpl;
};
} // namespace compound