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
	addListeners(config);
	if (listeners.size() == 0)
		throw "Cannot init server because there is no listener.";
	addErrorPages(config);
	addLocations(config);
	logger.debug("Server created");
}

void Server::addListeners(Config *config) {
	// Many listeners
	for (int i = 0; i < config->key_size("listen") && i < MAX_CONNECTION; i++) {
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
}

void Server::addErrorPages(Config *config) {
	// Many errorPages
	for (int i = 0; i < config->key_size("error_page") && i < MAX_CONNECTION; i++) {
		int status = util::stoi(config->get("error_page." + util::itos(i) + ".status_code"));
		if (status < 0) {
			logger.error("Bad status_code for error_page");
			continue;
		}
		error_pages[status] = config->get("error_page." + util::itos(i) + ".path");
		logger.debug("Error page for " + util::itos(status)
			+ ": " + config->get("error_page." + util::itos(i) + ".path"));
	}
	// Only one errorPage
	if (config->key_size("error_page") == 0) {
		int status = util::stoi(config->get("error_page.status_code"));
		if (status < 0) {
			logger.error("Bad status_code for error_page");
			return;
		}
		error_pages[status] = config->get("error_page.path");
		logger.debug("Error page for " + util::itos(status)
			+ ": " + config->get("error_page.path"));
	}
}

void Server::addLocations(Config *config) {
	// Many locations
	for (int i = 0; i < config->key_size("location"); i++) {
		Location *loc;
		try {
			loc = new Location(config->get("location." + util::itos(i) + ".route"));
		} catch (const char *message) {
			logger.error(message);
			continue;
		}
		loc->setRoot(config->get("location." + util::itos(i) + ".root"));
		loc->setDirectoryList(config->get("location." + util::itos(i) + ".directory_listing"));
		loc->setCgiExtension(config->get("location." + util::itos(i) + ".cgi"));
		// Add indexes
		for (int idx = 0; idx < config->key_size("location." + util::itos(i) + ".index"); idx++) {
			loc->addIndex(config->get("location." + util::itos(i) + ".index." + util::itos(idx)));
		}
		if (config->key_size("location." + util::itos(i) + ".index") == 0)
			loc->addIndex(config->get("location." + util::itos(i) + ".index"));
		// Add methods
		for (int idx = 0; idx < config->key_size("location." + util::itos(i) + ".allow_method"); idx++) {
			loc->addMethod(config->get("location." + util::itos(i) + ".allow_method." + util::itos(idx)));
		}
		if (config->key_size("location." + util::itos(i) + ".allow_method") == 0)
			loc->addMethod(config->get("location." + util::itos(i) + ".allow_method"));
		_locations.push_back(loc);
	}
	// Only one location
	if (config->key_size("location") == 0) {
		Location *loc;
		try {
			loc = new Location(config->get("location.route"));
		} catch (const char *message) {
			logger.error(message);
			return;
		}
		loc->setRoot(config->get("location.root"));
		loc->setDirectoryList(config->get("location.directory_listing"));
		loc->setCgiExtension(config->get("location.cgi"));
		// Add indexes
		for (int idx = 0; idx < config->key_size("location.index"); idx++) {
			loc->addIndex(config->get("location.index." + util::itos(idx)));
		}
		if (config->key_size("location.index") == 0)
			loc->addIndex(config->get("location.index"));
		// Add methods
		for (int idx = 0; idx < config->key_size("location.allow_method"); idx++) {
			loc->addMethod(config->get("location.allow_method." + util::itos(idx)));
		}
		if (config->key_size("location.allow_method") == 0)
			loc->addMethod(config->get("location.allow_method"));
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
		std::string body = getErrorPage(response); 
		response.setBody(body);
		response.addHeader("Content-Length", util::itos(response.getBody().size()));
		response.addHeader("Content-Type", "text/html");
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
	else
	{
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

std::string Server::getErrorPage(Response &response) {
	std::string path;
	try {
		path = error_pages.at(response.getStatusCode());
	} catch (...) {
		logger.debug("There is no page in config for this error");
	}
	if (!path.size())
		return defaultErrorPage(response);

	std::stringstream page;
	std::ifstream file(path);
	if (file.good()) {
		page << file.rdbuf();
		file.close();
	}
	return page.str();
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

Response Server::handle_get(const Request& request, Location *loc) {
	Response response;
	
	std::string aux = request.getResource().substr(loc->getLocation().size());
	std::string file_path = loc->getRoot() + "/" + aux;

	logger.debug("File path: " + file_path);
	
	//First we check if it is a directory or a file
	if(file_path[file_path.size() - 1] == '/')
	{
		std::string absolute_path = "";
		std::vector<std::string>::iterator it;
	
		for(it = loc->getIndexBegin(); it != loc->getIndexEnd(); it++)
		{
			absolute_path = file_path + (*it);
			//We check if file exists
			std::ifstream file(absolute_path.c_str());
			if (file.good())
				break;
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
	std::string aux = request.getResource().substr(loc->getLocation().size());
	std::string file_path = loc->getRoot() + "/" + aux;
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

	std::string aux = request.getResource().substr(loc->getLocation().size());
	std::string file_path = loc->getRoot() + "/" + aux;

	logger.debug("File path: " + file_path);

	if(file_path[file_path.size() - 1] == '/')
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
	std::string aux = request.getResource().substr(loc->getLocation().size());
	std::string file_path = loc->getRoot() + "/" + aux;

	logger.debug("File path: " + file_path);
	
	//Now we check if what we have received is a file or directory
	if(file_path[file_path.size() - 1] != '/')
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
