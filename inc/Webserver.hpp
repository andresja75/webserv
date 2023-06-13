#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include "XML/XMLDocument.hpp"
#include "macros.hpp"
#include "Server.hpp"

class Webserver {
private:
	XMLDocument config;

//	Logger logger;

	std::vector<Server *> servers;

public:
	Webserver();
	explicit Webserver(const XMLDocument& config);
	Webserver(const Webserver &copy);
	Webserver &operator=(const Webserver &copy);
	~Webserver();

	void run();

	bool initServer(Server *server, XMLElement *element);
};


#endif //WEBSERV_HPP
