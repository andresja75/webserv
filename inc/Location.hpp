
#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>

class Location
{
	private:
		std::string _location;//This is the target location
		std::string _root;
		std::vector<std::string> _indexes;
		std::vector<std::string> _allow_methods;
		bool _directory_listing;
		std::string _cgi_extension;
		std::map<unsigned int, std::string> _error_pages;//Correlation between status codes
														 //and given error file respects to
														 //this location
														 //ex: 404 ./error_404.html

	public:
		//Constructors and destructor
		Location();
		Location(std::string location);
		
		//Setters
		void setRoot(std::string root);
		void setDirectoryList(std::string root);
		void setCgiExtension(std::string cgi);
		bool addIndex(std::string index);
		bool addMethod(std::string method);
		bool addErrorPage(unsigned int status_code, std::string &error_file);

		//Getters
		std::string getLocation(void) const;
		std::string getRoot(void) const;
		std::string getCgiExtension(void) const;
		bool getDirectoryList(void) const;
		std::vector<std::string>::iterator getIndexBegin(void);
		std::vector<std::string>::iterator getIndexEnd(void);
		std::vector<std::string>::iterator getAllowMethodsBegin(void);
		std::vector<std::string>::iterator getAllowMethodsEnd(void);
		bool checkErrorFile(unsigned int status_code);
		std::string getErrorFile(unsigned int status_code);

		//Check if method is available
		bool checkMethod(std::string method);
};
#endif
