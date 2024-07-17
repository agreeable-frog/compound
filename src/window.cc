#include "compound/window.hh"
#include "window.hh"

#include <log4cplus/log4cplus.h>


namespace compound {
namespace impl {
    Window::Window(const std::string& name, size_t width, size_t height) {
        LOG4CPLUS_TRACE(_logger, "constructor");
    }
    Window::Window(const Window& other) {
        LOG4CPLUS_TRACE(_logger, "constructor");
    }
    Window& Window::operator=(const Window& other) {

    }
    Window::~Window() = default;
} // namespace impl
    Window::Window(const std::string& name, size_t width, size_t height) {
        _pImpl = std::unique_ptr<impl::Window>(new impl::Window(name, width, height));
    }
    Window::Window(const Window& other) {

    }
    Window& Window::operator=(const Window& other) {

    }
    Window::~Window() = default;
} // namespace compound