#pragma once

#include "log4cplus/log4cplus.h"
#include <string>
#include <memory>

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

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Program.public");
    std::unique_ptr<impl::Program> _pImpl;
};
} // namespace compound