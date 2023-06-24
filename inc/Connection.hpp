#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <cstring>
# include <string>
# include <map>
# include <ctime>
# include <unistd.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include "util.hpp"

# define READ_BUFFER_SIZE 1024
# define TIMEOUT 30

class Connection
{
public:
    Connection(int socket);
    Connection(int socket, const std::string &max_size);
    Connection(int socket, const std::string &max_size, struct sockaddr_in address);

private:
    Connection();

    int _socket;
    struct sockaddr_in _address; // Save client address but we don't use it
    std::string _request;
    std::string _response;
    size_t send_pos;
    bool finish_request;
    int _max_request_size;
    time_t init;

public:
	int index;
    int getSocket() const;
    const std::string &getRequest();
    const std::string &getResponse();
    bool isFinishRequest() const;
    bool checkTimeout();
    void setResponse(std::string resp);
    bool completeRequest();
    ssize_t recv();
    ssize_t send();
};

#endif
