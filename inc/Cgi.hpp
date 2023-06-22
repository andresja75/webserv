#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <csignal>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "request/Request.hpp"

#define CGI_BUFSIZE 20
#define CGI_TIMELIMIT 30

std::string executeCgi(const Request &request, const std::string &cgiBinPath, std::string file_content);

#endif