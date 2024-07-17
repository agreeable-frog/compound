#pragma once

#include <log4cplus/logger.h>
#include <log4cplus/log4cplus.h>

class TraceCalls {
    private:
        virtual log4cplus::Logger logger() {
            return log4cplus::Logger::getInstance("");
        };
    public:
        TraceCalls() {
            LOG4CPLUS_TRACE(logger(), "Constructor called");
        }
};