#include "../inc/Server.hpp"

Server::Server() {
	logger.debug("Init server");
	this->listeners.push_back(Listener("0.0.0.0", 80));
	this->name = "Server";

	logger.debug("Server created");
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

	logger.debug("Init server");
	if (!config)
		throw "There is no config for the server";

	std::string name = config->get("name");
	this->name = name.size() ? name : "Server";
	this->root_path = config->get("root");
	this->cgi_path = config->get("cgi_path");
	this->max_request_size = config->get("max_request_size");
	this->timeout = config->get("timeout");
	addListeners(config);
	if (listeners.size() == 0)
		throw "Cannot init server because there is no listener.";
	addErrorPages(config);
	addLocations(config);
	logger.debug("Server created");
}

void Server::addListeners(Config *config) {
	std::vector<Config*> confs = config->get_list("listen");
	std::vector<Config*>::iterator it;
	for (it = confs.begin(); it != confs.end() && listeners.size() < MAX_CONNECTION; it++) {
		try {
			Listener listener(
				(*it)->get("host"),
				util::stoi((*it)->get("port"))
			);
			listeners.push_back(listener);
			logger.log("Listen on " + (*it)->get("host") + ":" + (*it)->get("port"));
		} catch (const char *message) {
			logger.error(message);
		}
	}
}

void Server::addErrorPages(Config *config) {
	std::vector<Config*> confs = config->get_list("error_page");
	std::vector<Config*>::iterator it;
	for (it = confs.begin(); it != confs.end(); it++) {
		int status = util::stoi((*it)->get("status_code"));
		if (status < 0) {
			logger.error("Bad status_code for error_page");
			continue;
		}
		error_pages[status] = (*it)->get("path");
		logger.debug("Error page for " + util::itos(status) + ": " + (*it)->get("path"));
	}
}

void Server::addLocations(Config *config) {
	std::vector<Config*> confs = config->get_list("location");
	std::vector<Config*>::iterator it;
	for (it = confs.begin(); it != confs.end(); it++) {
		Location *loc;
		try {
			loc = new Location((*it)->get("route"));
		} catch (const char *message) {
			logger.error(message);
			continue;
		}
		loc->setRoot((*it)->get("root"));
		loc->setDirectoryList((*it)->get("directory_listing"));
		loc->setCgiExtension((*it)->get("cgi"));
		loc->setMaxBodySize((*it)->get("max_body_size"));
		// Add indexes
		for (int idx = 0; idx < (*it)->key_size("index"); idx++) {
			loc->addIndex((*it)->get("index." + util::itos(idx)));
		}
		if ((*it)->key_size("index") == 0)
			loc->addIndex((*it)->get("index"));
		// Add methods
		for (int idx = 0; idx < (*it)->key_size("allow_method"); idx++) {
			loc->addMethod((*it)->get("allow_method." + util::itos(idx)));
		}
		if ((*it)->key_size("allow_method") == 0)
			loc->addMethod((*it)->get("allow_method"));
		_locations.push_back(loc);
	}
	// No location in config
	if (config->key_size("location") == -1) {
		_locations.push_back(new Location());
	}
}

std::vector<Listener> *Server::getListeners() { return &listeners; }
std::vector<Connection *> *Server::getConnections() { return &connections; }
std::string &Server::getMaxSize() { return max_request_size; }
std::string &Server::getTimeout() { return timeout; }

Response Server::getResponse(const std::string &bufferstr) {

	Response response;
	if (bufferstr.empty()) {
		response = Response(400);
		logger.debug("Empty request");
	} else if (bufferstr.find('\r') == std::string::npos) {
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
		putErrorPage(response);
	}

	// logger.debug("Response raw: " + response.toString());
	// logger.debug("Content-Length: " + response.getHeader("Content-Length"));

	return response;
}

Response Server::handle_request(Request request) {

	logger.log("Request: " + request.getMethod() + " " + request.getPath());

	Location *loc = getLocation(request);
	Response response;

	//This means resource is not found	
	if(loc == NULL)
		response.setStatusCode(404);
	else if(loc->checkMethod(request.getMethod()) == false)
		response.setStatusCode(405);
	else if (loc->getMaxBodySize() > 0 && request.getBodySize() > loc->getMaxBodySize()) {
		response.setStatusCode(413);
	} else {
		if (request.getMethod() == "GET") {
			response = handle_get(request, loc);
		} else if (request.getMethod() == "POST") {
			response = handle_post(request, loc);
		} else if (request.getMethod() == "DELETE") {
			response = handle_delete(request, loc);
		} else if (request.getMethod() == "PUT") {
			response = handle_put(request, loc);
		} else {
			response.setStatusCode(405);
		}
	}

	response.addHeader("Connection", "close");
	response.addHeader("Server", name);
	response.addHeader("Date", util::getDate());

	return response;
}

void Server::putErrorPage(Response &response) {
	std::string body;
	std::string path;
	try {
		path = error_pages.at(response.getStatusCode());
	} catch (...) {
		logger.debug("There is no page in config for this error");
	}

	if (!path.size()) {
		body = defaultErrorPage(response);
	}
	else {
		std::stringstream page;
		std::ifstream file(path.c_str());
		if (file.good()) {
			page << file.rdbuf();
			file.close();
		}
		body = page.str();
	}

	response.setBody(body);
	response.addHeader("Content-Length", util::itos(response.getBody().size()));
	response.addHeader("Content-Type", "text/html");
}

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

std::string Server::get_file_path(const Request& request, Location *loc) {
	int len = loc->getLocation().size();
	if (request.getResource().size() < loc->getLocation().size())
		len--;
	std::string aux = request.getResource().substr(len);
	return util::joinPaths(loc->getRoot(), aux);
}

Response Server::handle_get(const Request& request, Location *loc) {
	Response response;
	std::string file_path = get_file_path(request, loc);

	logger.debug("File path: " + file_path);
	
	//First we check if it is a directory or a file
	if(util::isDir(file_path))
	{
		std::string absolute_path = "";
		std::vector<std::string>::iterator it;
	
		for(it = loc->getIndexBegin(); it != loc->getIndexEnd(); it++)
		{
			absolute_path = util::joinPaths(file_path, (*it));
			//We check if file exists
			std::ifstream file(absolute_path.c_str());
			if (file.good()) {
				file.close();
				break;
			}
			absolute_path = "";
		}
		
		//We check if iterator is pointing end, meaning file could not be found
		if(it != loc->getIndexEnd())
		{
			//if exists we introduce the content in the body
			std::ifstream file(absolute_path.c_str());
			std::string file_content;
			std::string line;
			while (std::getline(file, line, '\n')) 
			{
				file_content += line + "\n";
			}
			file.close();
			response.setBody(file_content);
		}
		else if(loc->getDirectoryList() == true)
		{
			std::string body = util::getDirectoryList(file_path);
			response.setBody(body);
			return response;
		}else
		{	
			return Response(404);	
		}
	}
	//if it doesnt pass through if, then comes here, it means target is a file
	else
	{
		//We check if file exists
		std::ifstream file(file_path.c_str());
		if (!file.good()) {
			return Response(404);
		}

		//if file exists we introduce the content in the body
		std::string file_content;
		std::string line;
		while (std::getline(file, line, '\n')) {
			file_content += line + "\n";
		}
		file.close();
		if (cgi_path.size() && loc->getCgiExtension().size()
			&& util::get_extension(file_path) == loc->getCgiExtension()) {
			file_content = executeCgi(request, cgi_path, file_content);
			// Quito los headers del cgi_tester
			if (file_content.find("\r\n\r\n") + 4 < file_content.size()) {
				file_content = file_content.substr(file_content.find("\r\n\r\n") + 4);
			}
		}
		response.setBody(file_content);
	}
	return response;
}

Response Server::handle_post(const Request& request, Location *loc) {
	Response response;
	std::string file_path = get_file_path(request, loc);
	std::string file_content = request.getBody();

	if (cgi_path.size() && loc->getCgiExtension().size()
		&& util::get_extension(file_path) == loc->getCgiExtension()) {
		file_content = executeCgi(request, cgi_path, file_content);
		// Quito los headers del cgi_tester
		if (file_content.find("\r\n\r\n") + 4 < file_content.size()) {
			file_content = file_content.substr(file_content.find("\r\n\r\n") + 4);
		}
	}
	response.setBody(file_content);
	return response;
}

//This function handle delete verb
Response Server::handle_delete(const Request& request, Location *loc) {
	Response response;
	std::string file_path = get_file_path(request, loc);

	logger.debug("File path: " + file_path);

	if(util::isDir(file_path))
		return response.setStatusCode(400);
	
	if(access(file_path.c_str(), F_OK) == 0)
	{
		//File exists
		if(remove(file_path.c_str()) == 0)
		{
			//File deleted ok
			response.setStatusCode(200);
		}
		else
		{
			//File deleted not ok
			response.setStatusCode(500);
		}

		//Here we should check going back through directories which one
		//are empty
	}
	else
	{
		//File not exists
		response.setStatusCode(404);
	}

	return response;
}

//This function handle put verb
Response Server::handle_put(Request& request, Location *loc) {
	Response response(201);
	std::string file_path = get_file_path(request, loc);

	logger.debug("File path: " + file_path);
	
	//Now we check if what we have received is a file or directory
	if(!util::isDir(file_path))
	{
		//If it reach this point is a file
		//Making directory if not exists
		std::string directory = file_path.substr(0, file_path.rfind("/"));
		directory = directory + "/";
		std::string aux_directory = "";
		unsigned long pos;
		pos = 0;
		while(directory.compare(aux_directory) != 0)
		{
			pos = directory.find("/", pos);
			if(pos == std::string::npos)
				break;
			pos++;
			aux_directory = directory.substr(0 , pos);
			struct stat st;
			if (aux_directory.size() && stat(aux_directory.c_str(), &st) == -1)
				mkdir(aux_directory.c_str(), 0700);
			pos++;
		}
			
		//Check if file already exist, if exist then 204 code, if not 201 code
		if(access(file_path.c_str(), F_OK) == 0)
			response.setStatusCode(204);
		else
			response.setStatusCode(201);

		//Now introduce the content on target file, and create resource
		std::ofstream file(file_path.c_str());
		if (!file.is_open())
			return Response(500);

		std::string file_content = request.getBody();
		//Check if body is chunked
		std::string header_chunked = request.getHeader("Transfer-Encoding");
		if (header_chunked.compare("chunked") == 0) {
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
			//If it is not chunked introduce content
			file << file_content;
		}
		file.close();
	}
	else
	{
		//If we reach this point is a directory
		//It means we are trying to create a directory
		response.setStatusCode(400);
	}

	return response;
}

Location* Server::getLocation(const Request& request)
{
	std::string file_path = request.getResource();
	std::string aux = "";
	std::string location;
	Location *l;
	unsigned long len;
	unsigned long coincidence;

	coincidence = 0;
	l = NULL;
	for(std::vector<Location *>::iterator it = this->_locations.begin();
			it != this->_locations.end(); it++)
	{
		location = (*it)->getLocation();
		len = location.size();
		if (location[len - 1] == '/') {
			len--;
			location = location.substr(0, len);
		}
		aux = file_path.substr(0, len);

		if(location.compare(aux) == 0)
		{
			if(l == NULL || len > coincidence)
			{
				coincidence = len;
				l = (*it);
			}
		}
	}

	return l;
}
