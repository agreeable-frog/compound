#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include <log4cplus/logger.h>

namespace compound::impl {
class Window {
    friend ::compound::Window;
public:
    Window(const std::string& name, size_t width, size_t height);
    Window(const Window&);
    Window& operator=(const Window&);
    ~Window();

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Window.private");
    std::string _name;
    size_t _width;
    static int _glfwInit;
    size_t _height;
    GLFWwindow* _handle;
    void init();
    std::map<int, bool> _keyStates;
    static void keyCallback(GLFWwindow* window, int key, int scancode,
                            int action, int mods);
    void makeContextCurrent();
    void pollEvents();
    bool shouldClose() const;
    void swapBuffers();
    const std::map<int, bool>& keyStates() const;
};
} // namespace compound::impl