#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include "log4cplus/logger.h"

#include "imgui.h"
#include "framebuffer.hh"

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
    static int _glfwInit;
    static GLFWwindow* _mainHandle;
    std::string _name;
    size_t _width;
    size_t _height;
    GLFWwindow* _handle;
    void init();
    std::map<int, bool> _keyStates;
    std::map<int, bool> _mouseButtonStates;
    std::array<double, 2> _cursorMove;
    static void keyCallback(GLFWwindow* window, int key, int scancode,
                            int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action,
                                    int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    void makeContextCurrent();
    void TMPsetViewPortToWindow();
    void TMPclear();
    double TMPgetTime() const;
    void pollEvents();
    bool shouldClose() const;
    void swapBuffers();
    const std::map<int, bool>& keyStates() const;
};
} // namespace compound::impl