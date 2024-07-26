#include "compound/window.hh"

#include <iostream>
#include <log4cplus/configurator.h>
#include <log4cplus/log4cplus.h>
#include <unistd.h>

int main() {
    log4cplus::initialize();
    log4cplus::PropertyConfigurator::doConfigure(std::string(LOG4CPLUS_CONFIG));
    log4cplus::Logger testLogger = log4cplus::Logger::getInstance("TEST");
    LOG4CPLUS_INFO(testLogger, "constructor");
    compound::Window window("test", 1200, 500);
    LOG4CPLUS_INFO(testLogger, "copy constructor");
    compound::Window window2(window);
    LOG4CPLUS_INFO(testLogger, "copy assignement");
    window2 = window;
    LOG4CPLUS_INFO(testLogger, "destructor");
    return 0;
}