
#if !defined(REQUEST_HPP)
#define REQUEST_HPP


#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "../util.hpp"
#include "../Logger.hpp"

#define SUPPORTED_HTTP_PROTOCOL "HTTP/1.1"

class Request
{
	private:
		std::string _method;
		std::string _protocol;
		std::string _path;
		std::string _resource;
		std::multimap<std::string, std::string> _query_params;
		std::map<std::string, std::string> _header_fields;
		std::string _body;

		//------- Private methods of Request ----------
		bool parse_request_line(std::string &request_line);
		void parse_and_insert_header(std::string header);
		void insert_body(const std::string &body);
	public:

		//Error class RequestException
		class RequestException: public std::exception
		{
			private:
				std::string _msg;
			public:
				RequestException(const std::string &msg);
				RequestException(void);
				~RequestException() throw();
				const char* what() const throw();
				
		};

		//------------ Constructor and destructor -------------
		Request(const std::string &str);
		
		//------------- Public methods ------------------

		//Method to parse the url, and get the resource and query params
		bool parse_url(void);
		void show_headers_info(void);
		std::string getHeader(std::string header) const;
		const std::map<std::string, std::string> *getHeaders() const;

		//Method to give information about request
		std::string getMethod(void) const;
		std::string getPath(void) const;
		std::string getResource(void) const;
		std::string getProtocol(void) const;
		std::string getBody(void) const;
		int getBodySize(void) const;
};


#endif
