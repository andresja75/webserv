
#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <algorithm>
#include <vector>
#include <iostream>

class Location
{
	private:
		std::string _location;//This is the target location
		std::string _root;
		std::vector<std::string> _indexes;
		std::vector<std::string> _allow_methods;
		bool _directory_listing;

	public:
		//Constructors and destructor
		Location(std::string location);
		
		//Setters
		void setRoot(std::string root);
		void setDirectoryList(std::string root);
		bool addIndex(std::string index);
		bool addMethod(std::string method);

		//Getters
		std::string getLocation(void) const;
		std::string getRoot(void) const;
		bool getDirectoryList(void) const;
		std::vector<std::string>::iterator getIndexBegin(void);
		std::vector<std::string>::iterator getIndexEnd(void);
		std::vector<std::string>::iterator getAllowMethodsBegin(void);
		std::vector<std::string>::iterator getAllowMethodsEnd(void);

		//Check if method is available
		bool checkMethod(std::string method);
};
#endif
