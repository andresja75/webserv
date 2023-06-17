#ifndef SERVER_HPP
#define SERVER_HPP


#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <unistd.h>
# include <vector>
#include "Logger.hpp"
#include "request/Request.hpp"
#include "response/Response.hpp"
// #include "MimeTypes.hpp"
// #include "Route.hpp"
#include "Listener.hpp"
#include "Connection.hpp"
#include "util.hpp"

#define MAX_CONNECTION 1024

class Server {

private:
	std::vector<Listener> listeners;
	std::vector<Connection> connections;
	std::string name;

	std::map<int, struct sockaddr_in> client_addresses;

	std::string root_path;
	//std::map<std::string, Route> routes;

	//Response handle_request(Request buffer);

public:


	Server();

	Server(const std::vector<std::pair<std::string, int> > &listen,
		   const std::string &name);

	~Server();

	const std::string &getRootPath() const;

	void setRootPath(const std::string &rootPath);

	int run();

	std::string getErrorPage(int status);

//	void init();

	void initDefaultErrorPages();

	Response getResponse(const std::string &bufferstr);

	//void addRoute(const Route &route);

	void setErrorPage(int status, const std::string& path);

	void setIndex(const std::string& index);

	//Response handle_get(const Request& request, const std::string& path);

	//Response handle_post(const Request& request, const std::string& path);

	//Response handle_delete(const Request& request, const std::string& path);

	//Response handle_put(const Request& request, const std::string& path);

	//std::string getCgiPath(const std::string &file_path);
};

#endif
