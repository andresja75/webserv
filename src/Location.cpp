
#include "../inc/Location.hpp"

//Class Location
//Constructors and destructor of Location class

Location::Location(std::string location)
	:_location(location), _root("")
{

}


//Public methods of Location class

//Setters
//This method introduces the specified root in the vector
void Location::setRoot(std::string root)
{
	this->_root = root;		
}

//This method introduces the specified index in the vector
bool Location::addIndex(std::string index)
{
	std::vector<std::string>::iterator it =
		std::find(this->_indexes.begin(), this->_indexes.end(), index);
	if(it == this->_indexes.end())
		return false;

	this->_indexes.push_back(index);
	return true;
}

//This method set what verbs is supported in location
bool Location::addMethod(std::string method)
{
	std::vector<std::string>::iterator it =
		std::find(this->_allow_methods.begin(), this->_allow_methods.end(), method);
	if(it == this->_allow_methods.end())
		return false;

	this->_allow_methods.push_back(method);
	return true;
}

//Getters

//This method return the location
std::string Location::getLocation(void) const
{
	return this->_location;
}

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

