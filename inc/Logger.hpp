#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

class Logger {
public:
    Logger();
    Logger(bool debug);
private:
    bool _debug;

public:
    void log(const std::string message);
    void debug(const std::string message);
    void error(const std::string message);
};

extern Logger logger;
#endif