/*
También lo hice con poll en lugar de select
*/
int Server::run() {
	struct pollfd fds[MAX_CONNECTION + 1];
	int nfds = 0;

	std::memset(fds, 0, sizeof(fds));
	// Add sockets of the servers
	for (int n = 0; n < (int) listeners.size() && n < 1024; n++) {
		int socket = listeners[n].getSocket();
		fds[n].fd = socket;
		fds[n].events = POLLIN;
		nfds++;
	}
	// Add open connections
	std::vector<Connection>::iterator connectIt = connections.begin();
	for (; connectIt != connections.end(); connectIt++) {
		int client = connectIt->getSocket();
		fds[nfds].fd = client;
		connectIt->index = nfds;
		if (connectIt->isFinishRequest())
			fds[nfds].events = POLLOUT;
		else
			fds[nfds].events = POLLIN;
		nfds++;
	}

	int timeout = 1 * 1000;
	int retval = poll(fds, nfds, timeout);
	// if (retval == 0)
	// 	std::cout << "TIMEOUT" << std::endl;
	if (retval == -1) {
		logger.error("Error while waiting for socket");
		return -1;
	}
	if (retval) {
		// Check servers connections
		for (int n = 0; n < (int) listeners.size() && n < 1024; n++) {
			if (fds[n].revents & POLLIN && nfds < MAX_CONNECTION + 1) {
				int new_socket;
				if ((new_socket = listeners[n].newConnection()) < 0) {
					logger.error("Failed to accept connection");
					return -1;
				}
				client_addresses[new_socket] = listeners[n].getClientAddress();
				logger.debug("Accepted connection");
				int flags = fcntl(new_socket, F_SETFL, fcntl(new_socket, F_GETFL) | O_NONBLOCK);
				if (flags < 0) {
					logger.error("Failed to set socket to non-blocking");
					return -1;
				}
				connections.push_back(Connection(new_socket));
			}
		}

		// Check open connections
		connectIt = connections.begin();
		for (; connectIt != connections.end(); connectIt++) {
			int idx = connectIt->index;

			// If client closed connection
			if (idx >= 0 && fds[idx].revents & POLLHUP) {
				close(connectIt->getSocket());
				connections.erase(connectIt);
				break;
			}

			// If client not open
			if (idx >= 0 && fds[idx].revents & POLLNVAL) {
				connections.erase(connectIt);
				break;
			}

			if (idx >= 0 && fds[idx].revents & POLLIN) {
				// If the request is fully received
				if (connectIt->recv() == 0) {
					Response response = getResponse(connectIt->getRequest(), 0);   // TODO siempre se pasa 0, para que el address?
					connectIt->setResponse(response.toString());
					logger.log("Response: " + response.getStatusString(), 9);
				}
			}

			if (idx >= 0 && fds[idx].revents & POLLOUT) {
				// If the response is fully send
				if (connectIt->send() == 0) {
					usleep(2100);
					connections.erase(connectIt);
					break;
				}
			}
		}
	}
	return 0;
}