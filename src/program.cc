#include "compound/program.hh"
#include "program.hh"

#include <sys/stat.h>

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
        delete logData;
        delete data;
        throw std::runtime_error("Failed to compile : " + _path);
    }

    delete data;
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
    glAttachShader(_id, _pVertShader->_id);
    glAttachShader(_id, _pFragShader->_id);
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

Program::Program(std::shared_ptr<const ShaderModule> pVertShader,
                 std::shared_ptr<const ShaderModule> pFragShader)
    : _pVertShader(pVertShader), _pFragShader(pFragShader) {
    init();
}

Program::Program(const std::string& vertShaderPath,
                 const std::string& fragShaderPath)
    : _pVertShader(std::make_shared<ShaderModule>(vertShaderPath,
                                                  ShaderModule::Type::VERTEX)),
      _pFragShader(std::make_shared<ShaderModule>(
          fragShaderPath, ShaderModule::Type::FRAGMENT)) {
    init();
}

Program::Program(const Program& other)
    : _pVertShader(other._pVertShader), _pFragShader(other._pFragShader) {
    init();
}

Program& Program::operator=(const Program& other) {
    if (this == &other) return *this;
    destroy();
    _pVertShader = other._pVertShader;
    _pFragShader = other._pFragShader;
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
} // namespace compound