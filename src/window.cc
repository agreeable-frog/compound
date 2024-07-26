#include "compound/window.hh"
#include "window.hh"

#include <log4cplus/log4cplus.h>

namespace compound {
namespace impl {
int Window::_glfwInit = 0;
void Window::init() {
    if (!_glfwInit) {
        glfwInit();
    }
    _glfwInit++;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    _handle =
        glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwSetWindowUserPointer(_handle, this);
    glfwMakeContextCurrent(_handle);
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("GLEW init failed.");
    }
    glfwSwapInterval(0); // We will manage the framerate ourselves
    glfwSetKeyCallback(_handle, &Window::keyCallback);
}

Window::Window(const std::string& name, size_t width, size_t height)
    : _name(name), _width(width), _height(height) {
    init();
}

Window::Window(const Window& other)
    : _name(other._name), _width(other._width), _height(other._height) {
    init();
}

Window& Window::operator=(const Window& other) {
    if (this == &other) return *this;
    if (_handle) glfwDestroyWindow(_handle);
    _name = other._name;
    _width = other._width;
    _height = other._height;
    init();
    return *this;
}

Window::~Window() {
    if (_handle) glfwDestroyWindow(_handle);
    _glfwInit--;
    if (!_glfwInit) {
        glfwTerminate();
    }
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
    void* userPtr = glfwGetWindowUserPointer(window);
    Window* pWindow = (Window*)userPtr;
    if (action == GLFW_PRESS) {
        pWindow->_keyStates[key] = true;
    } else if (action == GLFW_RELEASE) {
        pWindow->_keyStates[key] = false;
    }
}
} // namespace impl

Window::Window(const std::string& name, size_t width, size_t height)
    : _pImpl(new impl::Window(name, width, height)) {
}

Window::Window(const Window& other) : _pImpl(new impl::Window(*other._pImpl)) {
}

Window& Window::operator=(const Window& other) {
    if (this == &other) return *this;
    *_pImpl = *other._pImpl;
    return *this;
}

Window::~Window() {
}
} // namespace compound