#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <iostream>
#include <sstream>
# include "../inc/util.hpp"
//#include "Request.hpp"
//#include "Logger.hpp"
//class Request;
#define CGI_BUFSIZE 20
#define CGI_TIMELIMIT 30

//static bool timeout = false;

/*
static void handle_alarm_signal(int signal) {
    if (signal == SIGALRM) {
        kill(0, SIGQUIT);
        timeout = true;
    }
}

static void setup_timer(long seconds) {
    std::signal(SIGQUIT, SIG_IGN);
    std::signal(SIGALRM, handle_alarm_signal);

    struct itimerval timer;
    timer.it_value.tv_sec = seconds;
    timer.it_value.tv_usec = 0;
    // Not repeat
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
}
*/
/*
std::string util::executeCgi(const Request &request, const std::string &cgiBinPath, std::string file_content) {
    // TODO: hacer algo como esto https://github.com/cclaude42/webserv/blob/master/srcs/cgi/CgiHandler.cpp
    std::string x_header = "HTTP_X_SECRET_HEADER_FOR_TEST=" + request.getHeader("X-Secret-Header-For-Test");
    char **env = new char*[5];
    env[0] = strdup("REQUEST_METHOD=POST");
    env[1] = strdup("SERVER_PROTOCOL=HTTP/1.1");
    env[2] = strdup("PATH_INFO=hola");
    env[3] = strdup(x_header.c_str());
    env[4] = NULL;

    pid_t		pid;
    std::stringstream newBody;

	std::ofstream file("cgiInput");
	if (request.getHeader("Transfer-Encoding").find("chunked") != std::string::npos) {
		size_t size = 1;
		int i = 0;
		while (size) {
			size_t count = file_content.find("\r\n", i) - i;
			size = util::hex_str_to_dec(file_content.substr(i, count));
			size_t start = file_content.find("\r\n", i) + 2;
			file << file_content.substr(start, size);
			i = start + size + 2;
		}
	} else {
		file << file_content;
	}
	file.close();

    int fdin = open("cgiInput", O_RDONLY);
    int fdout = open("cgiOutput", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    pid = fork();

    if (pid == -1)
    {
        std::cerr << "Fork crashed." << std::endl;
        return ("Status: 500\r\n\r\n");
    }
    else if (!pid)
    {
        char *argv[2] = {0};
        argv[0] = new char[cgiBinPath.size() + 1];
        strcpy(argv[0], cgiBinPath.c_str());

        dup2(fdin, STDIN_FILENO);
        dup2(fdout, STDOUT_FILENO);
        execve(cgiBinPath.c_str(), argv, env);
        std::cerr << "Execve crashed." << std::endl;
        if (write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15) < 0)
            return ("Status: 500\r\n\r\n");
        exit(-1);
    }
    else
    {
        setup_timer(CGI_TIMELIMIT);
        waitpid(-1, NULL, 0);

        close(fdin);
        close(fdout);

        if (timeout) {
            logger.error("CGI timeout");
            newBody << "";
        } else {
            std::ifstream file("cgiOutput");
            newBody << file.rdbuf();
            file.close();
        }

    }

    for (size_t i = 0; env[i]; i++)
        free(env[i]);
    delete[] env;
    std::remove("cgiInput");
    std::remove("cgiOutput");

    return (newBody.str());
}
*/
int util::stoi(std::string str)
{
    int num;
    std::stringstream ss;
    ss << str;
    ss >> num;
    return num;
}

std::string util::itos(int n) {
    std::stringstream ss;
    ss << n;
    return ss.str();
}