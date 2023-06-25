
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include "../util.hpp"

#define HTTP_PROTOCOL "HTTP/1.1"

class Response
{
	
	private:
		//Private variables
		std::map<std::string, std::string> _headers;
		static std::map<unsigned int, std::string> _message_status_code;
		unsigned int _status_code;
		std::string _protocol_version;
		std::string _body;
		unsigned int _body_length;

	public:	
		//Constructors and destructor
		Response(unsigned int status_code);
		Response(void);

		//Methods to init the status code	
		static std::map<unsigned int, std::string> init_message_status_code(void);

		//Getters
		unsigned int getStatusCode(void) const;
		std::string getStatusMessage(void) const;
		std::string getProtocolVersion(void) const;
		std::string getBody(void) const;
		unsigned int getBodyLength(void) const;
		std::string getHeader(std::string& header_key);
		void showHeaders(void);

		//Setters
		bool setStatusCode(unsigned int status_code);
		void setBody(std::string& body);
		bool addHeader(std::string header_key, std::string header_value);

		std::string toString();
};

#endif
