#include "compound/window.hh"

#include <iostream>
#include <log4cplus/configurator.h>
#include <log4cplus/log4cplus.h>
#include <unistd.h>

int main() {
    log4cplus::initialize();
    log4cplus::PropertyConfigurator::doConfigure(std::string(LOG4CPLUS_CONFIG));
    log4cplus::Logger testLogger = log4cplus::Logger::getInstance("TEST");
    LOG4CPLUS_INFO(testLogger, "TEST");
    compound::Window window("test", 1200, 500);
    compound::Window window2(window);
    compound::Window window3 = window;
    return 0;
}