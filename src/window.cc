#include "compound/window.hh"
#include "window.hh"
#include "imgui/imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "log4cplus/log4cplus.h"

namespace compound {
namespace impl {
int Window::_glfwInit = 0;
GLFWwindow* Window::_mainHandle = 0;
void Window::init() {
    if (!_glfwInit) {
        glfwInit();
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 1); // disable MSAA
    _handle = glfwCreateWindow(_width, _height, _name.c_str(), 0, _mainHandle);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    if (!_glfwInit) {
        _mainHandle = _handle;
        glfwMakeContextCurrent(_mainHandle);
    }
    glfwSetWindowUserPointer(_handle, this);
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("GLEW init failed.");
    }
    glfwSwapInterval(1);
    glfwSetKeyCallback(_handle, &Window::keyCallback);
    glfwSetMouseButtonCallback(_handle, &Window::mouseButtonCallback);
    glfwSetCursorPosCallback(_handle, &Window::cursorPosCallback);
    if (!_glfwInit) {
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init("#version 330 core");
        ImGui_ImplGlfw_InitForOpenGL(_handle, true);
    }
    ++_glfwInit;
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
    if (_handle) {
        glfwDestroyWindow(_handle);
    }
    _glfwInit--;
    if (!_glfwInit) {
        _mainHandle = 0;
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
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

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action,
                                 int mods) {
    void* userPtr = glfwGetWindowUserPointer(window);
    Window* pWindow = (Window*)userPtr;
    if (action == GLFW_PRESS) {
        pWindow->_mouseButtonStates[button] = true;
    } else if (action == GLFW_RELEASE) {
        pWindow->_mouseButtonStates[button] = false;
    }
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    void* userPtr = glfwGetWindowUserPointer(window);
    Window* pWindow = (Window*)userPtr;
    static double lastxpos = xpos;
    static double lastypos = ypos;
    pWindow->_cursorMove[0] += lastxpos - xpos;
    pWindow->_cursorMove[1] += lastypos - ypos;
    lastxpos = xpos;
    lastypos = ypos;
}

void Window::makeContextCurrent() {
    glfwMakeContextCurrent(_handle);
}

void Window::pollEvents() {
    _cursorMove = {0.0, 0.0};
    glfwPollEvents();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(_handle);
}

void Window::swapBuffers() {
    glfwSwapBuffers(_handle);
}

const std::map<int, bool>& Window::keyStates() const {
    return _keyStates;
}

void Window::TMPsetViewPortToWindow() {
    glViewport(0, 0, _width, _height);
}

void Window::TMPclear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

double Window::TMPgetTime() const {
    return glfwGetTime();
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

void Window::makeContextCurrent() {
    _pImpl->makeContextCurrent();
}

void Window::pollEvents() {
    _pImpl->pollEvents();
}

bool Window::shouldClose() const {
    return _pImpl->shouldClose();
}

void Window::swapBuffers() {
    _pImpl->swapBuffers();
}

const std::map<int, bool>& Window::keyStates() const {
    return _pImpl->keyStates();
}

const std::map<int, bool>& Window::mouseButtonStates() const {
    return _pImpl->_mouseButtonStates;
}

const std::array<double, 2>& Window::cursorMove() const {
    return _pImpl->_cursorMove;
}

void Window::TMPsetViewPortToWindow() {
    _pImpl->TMPsetViewPortToWindow();
}

void Window::TMPclear() {
    _pImpl->TMPclear();
}

double Window::TMPgetTime() const {
    return _pImpl->TMPgetTime();
}
} // namespace compound