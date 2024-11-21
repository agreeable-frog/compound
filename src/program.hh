
#pragma once

#include "log4cplus/log4cplus.h"
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>

namespace compound {
namespace impl {
class ShaderModule {
    friend Program;

public:
    enum class Type {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER
    };
    ShaderModule(const std::string& path, Type type);
    ShaderModule(const ShaderModule&);
    ShaderModule& operator=(const ShaderModule&);
    ~ShaderModule();
    GLuint id() const;

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.ShaderModule.private");
    uint _uuid;
    std::string _path;
    Type _type;
    GLuint _id;
    void init();
    void destroy();
};

class Program {
    friend ::compound::Program;

public:
    Program(const std::string& vertShaderPath,
            const std::string& fragShaderPath);
    Program(const Program&);
    Program& operator=(const Program&);
    ~Program();
    void bind();
    bool isBound() const;
    void setUniform(int location, const glm::mat4& data);
    void setUniform(int location, const glm::vec4& data);
    void setUniform(int location, float data);

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Program.private");
    GLuint _id;
    static GLuint _boundId;
    ShaderModule _vertShader;
    ShaderModule _fragShader;
    void init();
    void destroy();
};
} // namespace impl
} // namespace compound