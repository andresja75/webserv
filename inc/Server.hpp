#ifndef SERVER_HPP
#define SERVER_HPP


#include <string>
#include <vector>
#include "Logger.hpp"
#include "Config.hpp"
#include "request/Request.hpp"
#include "response/Response.hpp"
// #include "MimeTypes.hpp"
#include "Listener.hpp"
#include "Connection.hpp"
#include "util.hpp"
#include "Location.hpp"
#include "Cgi.hpp"

#define MAX_CONNECTION 1024

class Server {

private:
	std::vector<Listener> listeners;
	std::vector<Connection *> connections;
	std::vector<Location *> _locations;
	std::string name;
	std::string root_path;
	std::string cgi_path;
	std::string error_page;
	std::string max_request_size;

	Response handle_request(Request buffer);
	Location* getLocation(const Request &request);

public:


	Server();
	Server(Config *config);
	~Server();

	const std::string &getRootPath() const;
	std::string getErrorPage(Response &response);
	Response getResponse(const std::string &bufferstr);
	std::vector<Listener> *getListeners();
	std::vector<Connection *> *getConnections();

	void setRootPath(const std::string &rootPath);
	void setErrorPage(int status, const std::string& path);
	void setIndex(const std::string& index);

	void addListeners(Config *config);
	void addLocations(Config *config);

	std::string defaultErrorPage(Response &response);

	Response handle_get(const Request& request, Location *loc);

	//Response handle_post(const Request& request, const std::string& path);
	//Response handle_delete(const Request& request, const std::string& path);

	Response handle_put(Request& request, Location *loc);

	//std::string getCgiPath(const std::string &file_path);
};

#endif


