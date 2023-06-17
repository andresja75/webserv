#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <iostream>
#include "XML/XMLDocument.hpp"
#include "macros.hpp"
#include "Server.hpp"

class Cluster {
private:
	XMLDocument config;

	std::vector<Server *> servers;

public:
	Cluster();
	explicit Cluster(const XMLDocument& config);
	Cluster(const Cluster &copy);
	Cluster &operator=(const Cluster &copy);
	~Cluster();

	void run();

	bool initServer(Server *server, XMLElement *element);
};


#endif
