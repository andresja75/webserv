#include <sys/fcntl.h>
#include <sys/stat.h>
#include "../inc/Server.hpp"

Server::Server() {
	logger.debug("Init server");
	this->listeners.push_back(Listener("0.0.0.0", 80));
	this->name = "Server";

	init();
}

Server::~Server() {
	for(std::vector<Location *>::iterator it = this->_locations.begin(); 
			it != this->_locations.end(); it++)
	{
		delete (*it);
	}
	for(std::vector<Connection *>::iterator it = this->connections.begin();
			it != this->connections.end(); it++)
	{
		delete (*it);
	}
}

Server::Server(Config *config) {
	
	/*Borrar despues*/
	Location *l = new Location("/");
	l->setRoot("./www");
	l->addIndex("index.html");
	l->addIndex("prueba.html");
	this->_locations.push_back(l);
	Location *l1 = new Location("/directorio_1/");
	l1->setRoot("./paginas_2");
	l1->addIndex("index.html");

	this->_locations.push_back(l1);
	Location *l2 = new Location("/directorio_/");
	l2->setRoot("./paginas");
	l2->addIndex("index.html");
	l2->addIndex("noticias.html");
	this->_locations.push_back(l2);
	Location *l3 = new Location("/directorio_1/index.html/");
	l3->setRoot("./paginas");
	l3->addIndex("index.html");
	this->_locations.push_back(l3);
	/*-----------------*/

	logger.debug("Init server");
	if (!config)
		throw "There is no config for the server";

	std::string name = config->get("name");
	this->name = name.size() ? name : "Server";
	this->root_path = config->get("root");
	// Many listeners
	for (size_t i = 0; i < config->key_size("listen") && i < MAX_CONNECTION; i++) {
		try {
			Listener listener(
				config->get("listen." + util::itos(i) + ".host"),
				util::stoi(config->get("listen." + util::itos(i) + ".port"))
			);
			listeners.push_back(listener);
			logger.log("Listen on " + config->get("listen." + util::itos(i) + ".host")
				+ ":" + config->get("listen." + util::itos(i) + ".port"));
		} catch (const char *message) {
			logger.error(message);
		}
	}
	// Only one listener
	if (config->key_size("listen") == 0) {
		try {
			Listener listener(
				config->get("listen.host"),
				util::stoi(config->get("listen.port"))
			);
			listeners.push_back(listener);
			logger.log("Listen on " + config->get("listen.host")
				+ ":" + config->get("listen.port"));
		} catch (const char *message) {
			logger.error(message);
		}
	}
	if (listeners.size() == 0)
		throw "Cannot init server because there is no listener.";
	init();
}

void Server::init() {

	//initDefaultErrorPages();

	logger.debug("Server created");
}

std::vector<Listener> *Server::getListeners() { return &listeners; }
std::vector<Connection *> *Server::getConnections() { return &connections; }

Response Server::getResponse(const std::string &bufferstr) {

	Response response;
	if (bufferstr.empty()) {
		response = Response(400);
		logger.debug("Empty request");
	} else if (bufferstr.find('\r') == std::string::npos) { // TODO ??
		response = Response(400);
		logger.debug("Invalid request");
	} else {

		try {
			Request request(bufferstr);
			response = handle_request(request);
		} 
		catch(Request::RequestException &e)
		{	
			logger.error("Exception while handling request: " +  std::string(e.what()));
			response = Response(400);
		}
		catch (std::exception &e) {
			logger.error("Exception while handling request: " + std::string(e.what()));
			response = Response(500);
		}
	}

	if (response.getStatusCode() >= 400) {
		//response.setBody(getErrorPage(response.getStatusCode()));
		response.addHeader("Content-Length", util::itos(response.getBody().size()));
		response.addHeader("Content-Type", "text/html");
	}

	// logger.debug("Response raw: " + response.toString());
	// logger.debug("Content-Length: " + response.getHeader("Content-Length"));

	// TODO body de prueba
	/*
	std::string b = defaultErrorPage(response);
	response.setBody(b);*/
	////

	return response;
}
Response Server::handle_request(Request request) {

	// logger.log("Request: " + request.getMethod() + " " + request.getPath(), 9);

	Location *loc = getLocation(request);
	Response response;

	//This means resource is not found	
	if(loc == NULL)
		response.setStatusCode(404);
	else if(loc->checkMethod(request.getMethod()) == false)
		response.setStatusCode(405);
	else
	{
		if (request.getMethod() == "GET") {
			response = handle_get(request, loc);
		} /*else if (request.getMethod() == "POST") {
			response = handle_post(request, path);
		} else if (request.getMethod() == "DELETE") {
			response = handle_delete(request, path);
		} else if (request.getMethod() == "PUT") {
			response = handle_put(request, path);
		} */else {
			response.setStatusCode(405);
	}
}


	response.addHeader("Connection", "close");
	response.addHeader("Server", "webserver");
	//response.addHeader("Date", util::datetime("%a, %d %b %Y %H:%M:%S %Z"));

	return response;
}

/*
std::string Server::getErrorPage(int status) {
	return this->routes["*"].getErrorPage(status);
}
*/

std::string Server::defaultErrorPage(Response &response) {
	std::stringstream page;
	page << "<html>\n"
		<< " <body>\n"
		<< "  <h1>" << response.getStatusCode() << " " << response.getStatusMessage()
		<< "</h1>\n"
		<< " </body>\n"
		<< "</html>";

	return page.str();
}

/*
const std::string &Server::getRootPath() const {
	return root_path;
}

void Server::setRootPath(const std::string &rootPath) {
    if (rootPath.empty()) {
        // logger.error("Root path cannot be empty");
        return;
    } else if (rootPath[rootPath.length() - 1] != '/') {
        // logger.error("Root path must end with a slash");
        return;
    } else if (rootPath[0] != '/') {
        std::string absolutePathCwd = std::string(getcwd(NULL, 0));
        root_path = util::combine_path(absolutePathCwd, rootPath, true);
        return;
    }
	root_path = rootPath;
}

*/
/*
void Server::addRoute(const Route& route) {
	this->routes[route.getPath()] = route;
}

void Server::setErrorPage(int status, const std::string& path) {
	this->routes["*"].setErrorPage(status, path);
}

void Server::setIndex(const std::string& index) {
	this->routes["*"].setIndex(index);
}
*/

Response Server::handle_get(const Request& request, Location *loc) {
	Response response;
	
	std::string aux = request.getResource().substr(loc->getLocation().size());
	std::string file_path = loc->getRoot() + "/" + aux;

	logger.debug("File path: " + file_path);
	logger.debug("request path: " + request.getResource());
	
	//First we check if it is a directory or a file
	if(file_path[file_path.size() - 1] == '/')
	{
		std::string absolute_path = "";
		std::cout<<"Directory"<<std::endl;
		std::vector<std::string>::iterator it;
		
		for(it = loc->getIndexBegin(); it != loc->getIndexEnd(); it++)
		{
			absolute_path = file_path + (*it);
			std::cout<<absolute_path<<std::endl;
			//We check if file exists
			std::ifstream file(absolute_path.c_str());
			if (file.good())
				break;
			absolute_path = "";
		}

		//We check if iterator is pointing end meaning file could not be found
		if(it == loc->getIndexEnd())
		{
			// logger.error("File not found");
			return Response(404);	
		}
		

		//if exists we introduce the content in the body
		std::ifstream file(absolute_path.c_str());
		std::string file_content;
		std::string line;
		while (std::getline(file, line, '\n')) {
			file_content += line + "\n";
		}
		response.setBody(file_content);


	}
	//if it doesnt pass through if, then comes here, it means target is a file
	else
	{
		std::cout<<"File"<<std::endl;
		//We check if file exists
		std::ifstream file(file_path.c_str());
		if (!file.good()) {
			// logger.error("File not found");
			return Response(404);
		}

		//if file exists we introduce the content in the body
		std::string file_content;
		std::string line;
		while (std::getline(file, line, '\n')) {
			file_content += line + "\n";
		}
		response.setBody(file_content);
	}
/*
	if (file_path.find(getRootPath()) != 0) {
		// logger.error("Invalid path");
		return Response(403);
	}
	struct stat statbuf = {};
	if (stat(file_path.c_str(), &statbuf) != 0) {
		// logger.error("File not found");
		return Response(404);
	}
	if (S_ISDIR(statbuf.st_mode)) {
		file_path = util::combine_path(file_path, this->routes["*"].getIndex(), true);
		// logger.debug("File path if is dir: " + file_path);
		// logger.debug(this->routes["*"].getIndex());
	}

	// Check if file exists
	std::ifstream file(file_path.c_str());
	if (!file.good()) {
		// logger.error("File not found");
		return Response(404);
	}
	std::string file_content;
	std::string line;
	while (std::getline(file, line, '\n')) {
		file_content += line + "\n";
	}

	std::string cgiBinPath = getCgiPath(file_path);
	if (cgiBinPath.size()) {
		file_content = util::executeCgi(request, cgiBinPath, file_content);
		// Quito los headers del cgi_tester
		if (file_content.find("\r\n\r\n") + 4 < file_content.size()) {
			file_content = file_content.substr(file_content.find("\r\n\r\n") + 4);
		}
	}
	response.setBody(file_content);

	std::string content_type = MimeTypes::getType(file_path);
	response.addHeader("Content-Type", content_type);
*/
	return response;
}
/*
Response Server::handle_post(const Request& request, const std::string& path) {
	// TODO hardcodeo la configuracion para el tester
	if (request.getPath().find("post_body") != std::string::npos) {
		if (request.getBodySize() > 100)
			return Response(413);
	}
	else if (!request.getBodySize()) {
		return Response(405);
	}

	Response response(200);
	std::string file_path = util::combine_path(getRootPath(), path, true);
	std::string file_content = request.getBody();

	std::string cgiBinPath = getCgiPath(file_path);
	if (cgiBinPath.size()) {
		file_content = util::executeCgi(request, cgiBinPath, file_content);
		// Quito los headers del cgi_tester
		if (file_content.find("\r\n\r\n") + 4 < file_content.size()) {
			file_content = file_content.substr(file_content.find("\r\n\r\n") + 4);
		}
	}
	response.setBody(file_content);
	return response;
}

Response Server::handle_delete(const Request& request, const std::string& path) {
	Response response(405);
	UNUSED(request);
	UNUSED(path);

	return response;
}

Response Server::handle_put(const Request& request, const std::string& path) {
	Response response(201);
	std::string file_path = util::combine_path(getRootPath(), path, true);
	std::string directory = file_path.substr(0, file_path.rfind("/"));
	struct stat st = {};

	if (directory.size() && stat(directory.c_str(), &st) == -1)
		mkdir(directory.c_str(), 0700);
	std::ofstream file(file_path.c_str());
	if (!file.is_open())
		return Response(500);

	std::string file_content = request.getBody();
	if (request.getHeader("Transfer-Encoding").find("chunked") != std::string::npos) {
		size_t size = 1;
		int i = 0;
		while (size) {
			size_t count = file_content.find("\r\n", i) - i;
			size = util::hex_str_to_dec(file_content.substr(i, count));
			size_t start = file_content.find("\r\n", i) + 2;
			file << file_content.substr(start, size);
			i = start + size + 2;
		}
	} else {
		file << file_content;
	}
	file.close();

	return response;
}

// Return the path of the cgi binary or an empty string
std::string Server::getCgiPath(const std::string &file_path) {
	std::string::size_type n = file_path.rfind(".");
	if (n == std::string::npos)
        return "";
	std::string extension = file_path.substr(n);

	std::map<std::string, Route>::iterator it = routes.begin();
	for (; it != routes.end(); it++) {
		if (it->second.getCgiExtension() == extension)
			return it->second.getCgiBinPath();
	}
	return "";
}
*/

Location* Server::getLocation(const Request& request)
{
	std::string file_path = request.getResource();
	std::string aux = "";
	Location *l;
	unsigned long len;
	unsigned long coincidence;

	coincidence = 0;
	l = NULL;
	for(std::vector<Location *>::iterator it = this->_locations.begin();
			it != this->_locations.end(); it++)
	{
		len = (*it)->getLocation().size();
		aux = file_path.substr(0, len);

		if((*it)->getLocation().compare(aux) == 0)
		{
			if(l == NULL)
			{
				coincidence = len;
				l = (*it);
			}
			if(len > coincidence)
			{
				l = (*it);
				coincidence = len;
			}
		}
	}


	return l;
}
