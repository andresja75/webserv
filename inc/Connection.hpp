#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <string.h>
# include <string>
# include <unistd.h>
# include <map>
# include <sys/socket.h>
# include <arpa/inet.h>
# include "util.hpp"

# define READ_BUFFER_SIZE 1024

class Connection
{
public:
    Connection(int socket);
    Connection(int socket, struct sockaddr_in address);

private:
    Connection();

    int _socket;
    struct sockaddr_in _address;
    std::string _request;
    std::string _response;
    size_t send_pos;
    bool finish_request;

public:
	int index;
    int getSocket() const;
    const std::string &getRequest();
    const std::string &getResponse();
    bool isFinishRequest() const;
    void setResponse(std::string resp);
    bool completeRequest();
    ssize_t recv();
    ssize_t send();
};

#endif
