
#pragma once

#include "log4cplus/log4cplus.h"
#include <string>

namespace compound {
namespace impl {
class Program {
    friend ::compound::Program;

public:
    Program(const std::string& vertShaderPath,
            const std::string& fragShaderPath);
    Program(const std::string& vertShaderStr, const std::string& fragShaderStr);
    Program(const Program&);
    Program& operator=(const Program&);
    ~Program();

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Program.private");
    static uint _uuidGen;
    static uint _boundUuid;
    uint _uuid;
    void bind();
};
} // namespace impl
} // namespace compound