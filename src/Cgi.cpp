#include "../inc/Cgi.hpp"

static bool timeout = false;

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

static char **getEnv(Request &request) {
    std::map<std::string, std::string> *headers = request.getHeaders();
    char **env = new char*[4 + headers->size()];
    int i = 0;

    std::string str = "REQUEST_METHOD=" + request.getMethod();
    env[i++] = strdup(str.c_str());
    str = "SERVER_PROTOCOL=" + request.getProtocol();
    env[i++] = strdup(str.c_str());
    str = "PATH_INFO=" + request.getPath();
    env[i++] = strdup(str.c_str());

    std::map<std::string, std::string>::iterator it = headers->begin();
    for (; it != headers->end(); it++) {
        std::string key = it->first;
        for (size_t j=0; j < key.size(); j++) {
            key[j] = std::toupper(key[j]);
            if (key[j] == '-')
                key[j] = '_';
        }
        str = "HTTP_"; str += key.c_str(); str += "="; str += it->second;
        env[i++] = strdup(str.c_str());
    }
    env[i++] = NULL;

    return env;
}

std::string executeCgi(Request &request, const std::string &cgiBinPath, std::string file_content) {
    (void)file_content;

    char **env = getEnv(request);

    pid_t               pid;
    std::stringstream   newBody;

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
