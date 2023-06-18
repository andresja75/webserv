

#if !defined(UTIL_HPP)
#define UTIL_HPP

namespace util {
//std::string executeCgi(const Request &request, const std::string &cgiBinPath, std::string file_content);

int stoi(std::string content_length);
std::string itos(int n);
}

#endif
