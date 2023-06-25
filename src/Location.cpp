#include "../inc/Location.hpp"

//Class Location
//Constructors and destructor of Location class

Location::Location() :_location("/"), _root(".")
{
	this->_allow_methods.push_back("GET");
}

Location::Location(std::string location)
	:_location(location), _root("./")
{
	if (_location[_location.size() - 1] != '/')
		throw "The root of the location should finish with /"; 
	this->_allow_methods.push_back("GET");
}


//Public methods of Location class

//Setters
//This method introduces the specified root in the vector
void Location::setRoot(std::string root)
{
	this->_root = root;		
}

//This method is to set listing directory
void Location::setDirectoryList(std::string listing) {
	if (listing == "True" || listing == "true" || listing == "On" || listing == "on")
		this->_directory_listing = true;
	else
		this->_directory_listing = false;
}

void Location::setCgiExtension(std::string cgi) {
	this->_cgi_extension = cgi;
}

void Location::setMaxBodySize(std::string max_body) {
	this->_max_body_size = util::stoi(max_body);
}

//This method introduces the specified index in the vector
bool Location::addIndex(std::string index)
{
	std::vector<std::string>::iterator it =
		std::find(this->_indexes.begin(), this->_indexes.end(), index);
	if(it != this->_indexes.end())
		return false;

	this->_indexes.push_back(index);
	return true;
}

//This method set what verbs is supported in location
bool Location::addMethod(std::string method)
{
	std::vector<std::string>::iterator it =
		std::find(this->_allow_methods.begin(), this->_allow_methods.end(), method);
	if(it != this->_allow_methods.end())
		return false;

	this->_allow_methods.push_back(method);
	return true;
}

//This function add relations between error pages 
bool Location::addErrorPage(unsigned int status_code, std::string &error_file)
{
	std::map<unsigned int, std::string>::iterator it =
		this->_error_pages.find(status_code);

	if(it != this->_error_pages.end())
		return false;

	this->_error_pages.insert(std::pair<unsigned int, std::string>(status_code, error_file));
	return true;
}

//Getters

//This method return the location
std::string Location::getLocation(void) const
{
	return this->_location;
}

bool Location::getDirectoryList(void) const { return this->_directory_listing; }

//This function returns and iterator to the begin of indexes
std::vector<std::string>::iterator Location::getIndexBegin(void)
{
	return this->_indexes.begin();
}

//This function returns and iterator to the end of indexes
std::vector<std::string>::iterator Location::getIndexEnd(void)
{
	return this->_indexes.end();
}

//This function returns and iterator to the begin of allowed methods
std::vector<std::string>::iterator Location::getAllowMethodsBegin(void)
{
	return this->_allow_methods.begin();
}

//This function returns and iterator to the end of allowed methods
std::vector<std::string>::iterator Location::getAllowMethodsEnd(void)
{
	return this->_allow_methods.end();
}

//This function returns and the root path to resource
std::string Location::getRoot(void) const
{
	return this->_root;
}

std::string Location::getCgiExtension(void) const { return this->_cgi_extension; }
int Location::getMaxBodySize(void) const { return this->_max_body_size; }

//This function checks if there is a file for a given status code
bool Location::checkErrorFile(unsigned int status_code)
{
	std::map<unsigned int, std::string>::iterator it = this->_error_pages.find(status_code);

	if(it != this->_error_pages.end())
		return true;

	return false;
}

//This function takes a status code and return file path where file associated with error is
std::string Location::getErrorFile(unsigned int status_code)
{
	std::string error_page = "";

	std::map<unsigned int, std::string>::iterator it = this->_error_pages.find(status_code);
	if(it != this->_error_pages.begin())
		error_page = it->second;
	return error_page;
}


//This function checks if a given method is on location
bool Location::checkMethod(std::string method)
{
	std::vector<std::string>::iterator it = std::find(this->_allow_methods.begin(),
			this->_allow_methods.end(), method);

	if(it == this->_allow_methods.end())
		return false;
	return true;
}
