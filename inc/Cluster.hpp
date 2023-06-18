#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "../inc/Config.hpp"
#include "../inc/Server.hpp"
#include "../inc/Logger.hpp"

class Cluster {
private:
	Config _config;
	std::vector<Server *> _servers;

	int addSocketsToWatch(struct pollfd *fds);
	int checkListeners(struct pollfd *fds);
	void checkConnections(struct pollfd *fds);

public:
	Cluster();
	~Cluster();
	Cluster(const Config& config);

	void run();
	int init();
};

#endif
