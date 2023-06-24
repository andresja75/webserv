#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <sstream>

namespace util {
    //std::string executeCgi(const Request &request, const std::string &cgiBinPath, std::string file_content);

    int stoi(std::string str);
    std::string itos(int n);
    int hex_str_to_dec(const std::string &str);
    void remove_whitespaces(std::string &str);
}

#endif
