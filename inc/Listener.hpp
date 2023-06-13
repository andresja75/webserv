#ifndef LISTENER_HPP
# define LISTENER_HPP

# include <cstring>
# include <cstdlib>
# include <unistd.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include "Logger.hpp"

# define DEFAULT_IP "0.0.0.0"
# define DEFAULT_PORT 8080
# define DEFAULT_BACKLOG 100

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

class Listener
{
public:
    Listener();
    Listener(
        std::string ip, // Ip of the socket
        short port,     // Port of the socket
        int backlog=DEFAULT_BACKLOG     // Maximum length for the queue of pending connections.
    );

private:
    int _socket;
    int _backlog;
    SA_IN _client_address;
    socklen_t _addr_size;

public:
    int getSocket() const;
    SA_IN getClientAddress() const; // TODO Address methods should be on Connection
    std::string getClientIp() const;
    int getClientPort() const;
    int newConnection();
};

#endif
