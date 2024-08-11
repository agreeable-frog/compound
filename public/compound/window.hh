#pragma once

#include "log4cplus/log4cplus.h"
#include <map>
#include <memory>
#include <string>
#include <sys/types.h>

namespace compound {
namespace impl {
class Window;
}
class Window {
public:
    Window(const std::string& name, size_t width, size_t height);
    Window(const Window&);
    Window& operator=(const Window&);
    ~Window();
    void makeContextCurrent();
    void TMPsetViewPortToWindow();
    void TMPclear();
    void pollEvents();
    bool shouldClose() const;
    void swapBuffers();
    const std::map<int, bool>& keyStates() const;

private:
    log4cplus::Logger _logger =
        log4cplus::Logger::getInstance("compound.Window.public");
    std::unique_ptr<impl::Window> _pImpl;
};
} // namespace compound