#include "../inc/Logger.hpp"

Logger::Logger(): _debug(true) {}

Logger::Logger(bool debug): _debug(debug) {}

void Logger::set_debug(bool debug) { _debug = debug; }

void Logger::log(const std::string message) {
    std::cout << util::getDate("%Y-%m-%d %H:%M:%S")
        << " [" LIGHT_GREEN "LOG" RESET "]: " << message << std::endl;
}

void Logger::debug(const std::string message) {
    if (_debug) {
        std::cout << util::getDate("%Y-%m-%d %H:%M:%S")
            << " [" LIGHT_CYAN "DEBUG" RESET "]: " << message << std::endl;
    }
}

void Logger::error(const std::string message) {
    std::cout << util::getDate("%Y-%m-%d %H:%M:%S")
        << " [" LIGHT_RED "ERROR" RESET "]: " << message << std::endl;
}

Logger logger = Logger();