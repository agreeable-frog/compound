#include "compound/program.hh"
#include "program.hh"

#include <sys/stat.h>
#include <glm/gtc/type_ptr.hpp>

namespace compound {
namespace impl {
static std::string loadText(const std::string& path) {
    LOG4CPLUS_DEBUG(
        log4cplus::Logger::getInstance("compound.ShaderModule.private"),
        "Loading text in file " + path);
    struct stat s;
    if (!(stat(path.c_str(), &s) == 0 && (s.st_mode & S_IFREG))) {
        throw std::runtime_error(path + "is not a regular file");
    }
    std::ifstream inputSrcFile(path, std::ios::in);
    std::stringstream textStream;
    textStream << inputSrcFile.rdbuf();
    LOG4CPLUS_DEBUG(
        log4cplus::Logger::getInstance("compound.ShaderModule.private"),
        "Loaded text in file " + path);
    return textStream.str();
}

void ShaderModule::init() {
    std::string src = loadText(_path);
    _id = glCreateShader((GLenum)_type);
    char* data = new char[src.size() + 1];
    src.copy(data, src.size() + 1);
    data[src.size()] = '\0';
    glShaderSource(_id, 1, (const GLchar**)&(data), 0);
    GLint compileStatus = GL_TRUE;
    glCompileShader(_id);
    glGetShaderiv(_id, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLint logSize;
        glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &logSize);
        char* logData = new char[logSize];
        LOG4CPLUS_DEBUG(_logger, "TEST1");
        glGetShaderInfoLog(_id, logSize, &logSize, logData);
        LOG4CPLUS_FATAL(_logger, std::string("Failed to compile shader :\n") +
                                     std::string(logData));
        delete[] logData;
        delete[] data;
        throw std::runtime_error("Failed to compile : " + _path);
    }

    delete[] data;
}

void ShaderModule::destroy() {
    glDeleteShader(_id);
}

ShaderModule::ShaderModule(const std::string& path, ShaderModule::Type type)
    : _path(path), _type(type) {
    init();
}

ShaderModule::ShaderModule(const ShaderModule& other)
    : _path(other._path), _type(other._type) {
    init();
}

ShaderModule& ShaderModule::operator=(const ShaderModule& other) {
    if (this == &other) return *this;
    destroy();
    _path = other._path;
    _type = other._type;
    init();
    return *this;
}

ShaderModule::~ShaderModule() {
    destroy();
}

GLuint ShaderModule::id() const {
    return _id;
}

GLuint Program::_boundId = 0;

void Program::init() {
    _id = glCreateProgram();
    if (_id == 0) throw std::runtime_error("Program creation failed");
    glAttachShader(_id, _vertShader._id);
    glAttachShader(_id, _fragShader._id);
    glLinkProgram(_id);
    GLint linkStatus = GL_TRUE;
    glGetProgramiv(_id, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        GLint logSize;
        glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &logSize);
        char* logData = new char(logSize);
        if (logData != 0) {
            glGetProgramInfoLog(_id, logSize, &logSize, logData);
            delete logData;
        }
        throw std::runtime_error("Program link failed");
    }
}

void Program::destroy() {
    if (isBound()) _boundId = 0;
    glDeleteProgram(_id);
}

bool Program::isBound() const {
    return _boundId == _id;
}

Program::Program(const std::string& vertShaderPath,
                 const std::string& fragShaderPath)
    : _vertShader(vertShaderPath, ShaderModule::Type::VERTEX),
      _fragShader(fragShaderPath, ShaderModule::Type::FRAGMENT) {
    init();
}

Program::Program(const Program& other)
    : _vertShader(other._vertShader), _fragShader(other._fragShader) {
    init();
}

Program& Program::operator=(const Program& other) {
    if (this == &other) return *this;
    destroy();
    _vertShader = other._vertShader;
    _fragShader = other._fragShader;
    init();
    return *this;
}

Program::~Program() {
    LOG4CPLUS_TRACE(_logger,
                    "Destroying program with id : " + std::to_string(_id));
    destroy();
}

void Program::bind() {
    if (!isBound()) {
        LOG4CPLUS_DEBUG(_logger,
                        "Binding program with id " + std::to_string(_id));
        glUseProgram(_id);
        _boundId = _id;
    }
}

void Program::setUniform(int location, const glm::mat4& data) {
    if (!isBound()) {
        LOG4CPLUS_ERROR(_logger, "Cannot set uniform on not bound program.");
        return;
    }
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(data));
}

void Program::setUniform(int location, const glm::vec4& data) {
    if (!isBound()) {
        LOG4CPLUS_ERROR(_logger, "Cannot set uniform on not bound program.");
        return;
    }
    glUniform4fv(location, 1, glm::value_ptr(data));
}

void Program::setUniform(int location, float data) {
    if (!isBound()) {
        LOG4CPLUS_ERROR(_logger, "Cannot set uniform on not bound program.");
        return;
    }
    glUniform1fv(location, 1, &data);
}
} // namespace impl

Program::Program(const std::string& vertShaderPath,
                 const std::string& fragShaderPath)
    : _pImpl(new impl::Program(vertShaderPath, fragShaderPath)) {
}

Program::Program(const Program& other)
    : _pImpl(new impl::Program(*other._pImpl)) {
}

Program& Program::operator=(const Program& other) {
    if (this == &other) return *this;
    *_pImpl = *other._pImpl;
    return *this;
}

Program::~Program() {
}

void Program::bind() {
    _pImpl->bind();
}

void Program::setUniform(int location, const glm::mat4& data) {
    _pImpl->setUniform(location, data);
}

void Program::setUniform(int location, const glm::vec4& data) {
    _pImpl->setUniform(location, data);
}

void Program::setUniform(int location, float data) {
    _pImpl->setUniform(location, data);
}
} // namespace compound