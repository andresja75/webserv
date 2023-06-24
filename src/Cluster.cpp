#include "../inc/Cluster.hpp"

Cluster::Cluster() {}
Cluster::Cluster(const Config& config): _config(config) {}

Cluster::~Cluster() {
    for (size_t i = 0; i < _servers.size(); i++) {
        delete _servers[i];
    }
}

int Cluster::init() {
    logger.debug("Init Cluster");
    size_t size = _config.key_size("server");

    for (size_t i = 0; i < size; i++) {
        try {
            Server *server = new Server(_config.get_config("server." + util::itos(i)));
            _servers.push_back(server);
		} catch (const char *message) {
			logger.error(message);
        }
    }
    if (size == 0) {
        try {
            Server *server = new Server(_config.get_config("server"));
                _servers.push_back(server);
		} catch (const char *message) {
			logger.error(message);
        }
    }

    return !_servers.size();
}

int Cluster::addSocketsToWatch(struct pollfd *fds) {
    int nfds = 0;

    for (size_t i = 0; i < _servers.size(); i++) {
        // Add sockets of the servers
        std::vector<Listener> *listeners = _servers[i]->getListeners();
        for (size_t j = 0; j < listeners->size() && j < MAX_CONNECTION; j++) {
            int socket = (*listeners)[j].getSocket();
            (*listeners)[j].index = nfds;
            fds[nfds].fd = socket;
            fds[nfds].events = POLLIN;
            nfds++;
        }
        // Add open connections
        std::vector<Connection *> *connections = _servers[i]->getConnections();
        std::vector<Connection *>::iterator connectIt = connections->begin();
        for (; connectIt != connections->end(); connectIt++) {
            int client = (*connectIt)->getSocket();
            fds[nfds].fd = client;
            (*connectIt)->index = nfds;
            if ((*connectIt)->isFinishRequest())
                fds[nfds].events = POLLOUT;
            else
                fds[nfds].events = POLLIN;
            nfds++;
        }
    }
    return nfds;
}

int Cluster::checkListeners(struct pollfd *fds) {
    for (size_t i = 0; i < _servers.size(); i++) {
        // Check servers connections
        std::vector<Listener> *listeners = _servers[i]->getListeners();
        std::vector<Connection *> *connections = _servers[i]->getConnections();
        for (size_t n = 0; n < listeners->size() && n < MAX_CONNECTION; n++) {
            int idx = (*listeners)[n].index;
            if (idx >= 0 && fds[idx].revents & POLLIN) {
                int new_socket;
                if ((new_socket = (*listeners)[n].newConnection()) < 0) {
                    logger.error("Failed to accept connection");
                    return -1;
                }
                logger.debug("Accepted connection");
                int flags = fcntl(new_socket, F_SETFL, fcntl(new_socket, F_GETFL) | O_NONBLOCK);
                if (flags < 0) {
                    logger.error("Failed to set socket to non-blocking");
                    return -1;
                }
                connections->push_back(new Connection(new_socket, _servers[i]->getMaxSize(), (*listeners)[n].getClientAddress()));
            }
        }
    }
    return 0;
}

void Cluster::checkConnections(struct pollfd *fds) {
    for (size_t i = 0; i < _servers.size(); i++) {
        std::vector<Connection *> *connections = _servers[i]->getConnections();
        std::vector<Connection *>::iterator connectIt = connections->begin();
        connectIt = connections->begin();
        for (; connectIt != connections->end(); connectIt++) {
            int idx = (*connectIt)->index;

            // If client closed connection
            if (idx >= 0 && fds[idx].revents & POLLHUP) {
                logger.debug("Client close connection");
                close((*connectIt)->getSocket());
                delete *connectIt;
                connections->erase(connectIt);
                break;
            }

            // If client not open
            if (idx >= 0 && fds[idx].revents & POLLNVAL) {
                logger.debug("Client is not open");
                delete *connectIt;
                connections->erase(connectIt);
                break;
            }

            if (idx >= 0 && fds[idx].revents & POLLIN) {
                ssize_t r_recv = (*connectIt)->recv();
                // If the request is fully received
                if (r_recv == 0) {
                    Response response = _servers[i]->getResponse((*connectIt)->getRequest());
                    (*connectIt)->setResponse(response.toString());
                    logger.log("Response: " + util::itos(response.getStatusCode()));
                }
                // If reach maximum request size
                if (r_recv == -2) {
                    Response response(413);
                    _servers[i]->putErrorPage(response);
                    (*connectIt)->setResponse(response.toString());
                    logger.error("Reach maximun request size");
                    break;
                }
                // If an error in read
                if (r_recv < 0) {
                    delete *connectIt;
                    connections->erase(connectIt);
                    logger.error("Failed to read on socket");
                    break;
                }
            }

            if (idx >= 0 && fds[idx].revents & POLLOUT) {
                ssize_t r_send = (*connectIt)->send();
                // If the response is fully send
                if (r_send == 0) {
                    usleep(2100); // Limit for macs. Block too much traffic per second.
                    delete *connectIt;
                    connections->erase(connectIt);
                    break;
                }
                // If error in write
                if (r_send < 0) {
                    delete *connectIt;
                    connections->erase(connectIt);
                    logger.error("Failed to write on socket");
                    break;
                }
            }
        }
    }
}

void Cluster::run() {
    logger.debug("Run Cluster");

    while (true) {
        struct pollfd fds[MAX_CONNECTION + 1];
        std::memset(fds, 0, sizeof(fds));
        int nfds = addSocketsToWatch(fds);

        int timeout = 2 * 1000;
        int retval = poll(fds, nfds, timeout);
        // if (retval == 0)
        // 	std::cout << "TIMEOUT" << std::endl;
        if (retval == -1) {
            logger.error("Error while waiting for socket");
            return;
        }
        if (retval) {
            // Check servers connections
            checkListeners(fds);
            // Check open connections
            checkConnections(fds);
        }
    }
}
