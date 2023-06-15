
#if !defined(REQUEST_HPP)
#define REQUEST_HPP


#include <string>
#include <iostream>
#include <sstream>
#include <map>

#define SUPPORTED_HTTP_PROTOCOL "HTTP/1.1"

class Request
{
	private:
		std::string _method;
		std::string _protocol;
		std::string _url;
		std::string _resource;
		std::multimap<std::string, std::string> _query_params;

		//------- Private methods of Request ----------
		bool parse_request_line(std::string &request_line);

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
};


#endif
