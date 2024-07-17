#pragma once

#include <map>
#include <memory>
#include <string>
#include <sys/types.h>

namespace compound {
namespace impl {
class Window;
}
class Window {
private:
    std::unique_ptr<impl::Window> _pImpl;

public:
    Window(const std::string& name, size_t width, size_t height);
    Window(const Window&);
    Window& operator=(const Window&);
    ~Window();
    void makeContextCurrent();
    bool shouldClose() const;
    void swapBuffers();
    const std::map<int, bool>& keyStates() const;
};
} // namespace compound