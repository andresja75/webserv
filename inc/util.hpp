#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <sstream>
#include <dirent.h>
#include <ctime>
#include <sys/stat.h>

namespace util {
    //std::string executeCgi(const Request &request, const std::string &cgiBinPath, std::string file_content);

    int stoi(std::string str);
    std::string itos(int n);
    int hex_str_to_dec(const std::string &str);
    void remove_whitespaces(std::string &str);
    std::string get_extension(const std::string &str);
    std::string getDirectoryList(std::string file_path);
    std::string getDate();
    bool isDir(std::string path);
    std::string joinPaths(std::string root, std::string path);
}

#endif
