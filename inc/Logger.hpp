#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include "util.hpp"

#define RESET "\033[0m"

#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define DEFAULT "\033[39m"
#define GRAY "\033[90m"
#define LIGHT_RED "\033[91m"
#define LIGHT_GREEN "\033[92m"
#define LIGHT_YELLOW "\033[93m"
#define LIGHT_BLUE "\033[94m"
#define LIGHT_MAGENTA "\033[95m"
#define LIGHT_CYAN "\033[96m"
#define LIGHT_WHITE "\033[97m"

class Logger {
public:
    Logger();
    Logger(bool debug);
private:
    bool _debug;

public:
    void set_debug(bool debug);
    void log(const std::string message);
    void debug(const std::string message);
    void error(const std::string message);
};

extern Logger logger;
#endif