#include "../inc/Logger.hpp"

Logger::Logger(): _debug(true) {}

Logger::Logger(bool debug): _debug(debug) {}

void Logger::set_debug(bool debug) { _debug = debug; }

void Logger::log(const std::string message) {
    std::cout << "[LOG]: " << message << std::endl;
}

void Logger::debug(const std::string message) {
    if (_debug)
        std::cout << "[DEBUG]: " << message << std::endl;
}

void Logger::error(const std::string message) {
    std::cout << "[ERROR]: " << message << std::endl;
}

Logger logger = Logger();