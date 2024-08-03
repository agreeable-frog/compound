#include "compound/window.hh"

#include <iostream>
#include <log4cplus/configurator.h>
#include <log4cplus/log4cplus.h>
#include <unistd.h>

int main() {
    log4cplus::initialize();
    log4cplus::PropertyConfigurator::doConfigure(std::string(LOG4CPLUS_CONFIG));
    log4cplus::Logger testLogger = log4cplus::Logger::getInstance("TEST");
    compound::Window testWindow("test", 900, 600);
    testWindow.makeContextCurrent();
    while (!testWindow.shouldClose()) {
        testWindow.pollEvents();
        testWindow.swapBuffers();
    }
    return 0;
}