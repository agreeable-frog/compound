#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include <log4cplus/logger.h>

namespace compound::impl {
class Window {
public:
    Window(const std::string& name, size_t width, size_t height);
    Window(const Window&);
    Window& operator=(const Window&);
    ~Window();

private:
    log4cplus::Logger _logger = log4cplus::Logger::getInstance("compound.Window");
    GLFWwindow* _handle;
    std::string _name;
    size_t width;
    size_t height;
    std::map<int, bool> _keyStates;
};
} // namespace compound::impl