#include "compound/window.hh"
#include "compound/program.hh"

#include <iostream>
#include "log4cplus/configurator.h"
#include "log4cplus/log4cplus.h"
#include <unistd.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

int main() {
    log4cplus::initialize();
    log4cplus::PropertyConfigurator::doConfigure(std::string(LOG4CPLUS_CONFIG));
    log4cplus::Logger testLogger = log4cplus::Logger::getInstance("TEST");
    compound::Window testWindow("test", 900, 600);
    compound::Program testProgram(std::string(SHADERS_PATH) + "basic.vert",
                                  std::string(SHADERS_PATH) + "basic.frag");
    testProgram.bind();
    while (!testWindow.shouldClose()) {
        testWindow.makeContextCurrent();
        testWindow.pollEvents();
        testWindow.TMPsetViewPortToWindow();
        testWindow.TMPclear();
        testWindow.swapBuffers();
    }
    return 0;
}