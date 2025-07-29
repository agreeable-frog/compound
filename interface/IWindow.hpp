#pragma once

#include <memory>

namespace compound {
class IWindow {
public:

};

class WindowFactory {
    static std::shared_ptr<IWindow> build();
};
}