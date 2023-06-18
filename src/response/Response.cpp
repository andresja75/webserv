
#include "../../inc/response/Response.hpp"

std::map<unsigned int, std::string> Response::_message_status_code;
//Constructors and destructor of Response class

//Response constructor with unsigned int constructor
Response::Response(unsigned int status_code)
	:_status_code(status_code), _protocol_version(HTTP_PROTOCOL),
	_body(""), _body_length(0) 
{
	if(_message_status_code.size() <= 0)
		init();

	std::map<unsigned int, std::string>::iterator it = _message_status_code.find(status_code);

	if(it == _message_status_code.end())
	{
		this->_status_code = 500;
		this->_status_message = "Internal Server Error";

	}
	else
	{
		this->_status_message = it->second;
	}
}

//Response default constructor
Response::Response(void)
	:_status_code(200), _status_message("Ok"), _protocol_version(HTTP_PROTOCOL),
	_body(""), _body_length(0) 
{
}

//----------------------------------------------------------------------------
//Public methods of Response class

//This static method is used to initialize status codes
void Response::init(void)
{
	//100 status codes
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(100, "Continue"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(101, "Switching protocol"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(102, "Processing"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(103, "Early Hints"));

	//200 status codes
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(200, "Ok"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(201, "Created"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(202, "Accepted"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(203, "Non-Authorative Information"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(204, "No Content"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(205, "Reset Content"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(206, "Partial Content"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(207, "Multi-Status"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(208, "Multi-Status"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(226, "IM Used"));
	
	//300 status codes
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(300, "Multiple Choice"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(301, "Moved Permanently"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(302, "Found"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(303, "See Other"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(304, "Not Modified"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(305, "Use Proxy"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(307, "Temporary Redirect"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(308, "Permanent Redirect"));

	//400 status codes	
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(400, "Bad Request"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(401, "Unauthorized"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(403, "Forbidden"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(404, "Not Found"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(405, "Method Not Allowed"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(406, "Not Acceptable"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(407, "Proxy Authentication Required"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(408, "Request Timeout"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(409, "Conflict"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(410, "Gone"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(411, "Length Required"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(412, "Precondition Failed"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(413, "Payload Too Large"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(414, "URI Too Long"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(415, "Unsupported Media Type"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(416, "Requested Range Not Satisfiable"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(417, "Expectation Failed"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(418, "I'm a teapot"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(421, "Misredirect Request"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(422, "Unprocessable Entity"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(423, "Locked"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(424, "Failed Dependency"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(426, "Upgrade Required"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(428, "Precondition Required"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(429, "Too Many Requests"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(431, "Request Header Fields Too Large"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(451, "Unavailable For Legal Reasons"));

	//500 status codes	
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(500, "Internal Server Error"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(501, "Not Implemented"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(502, "Bad Gateway"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(503, "Service Unavailable"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(504, "Gateway Timeout"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(505, "HTTP Version Not Supported"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(506, "Variant Also Negotiates"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(507, "Insufficient Storage"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(508, "Loop Detected"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(510, "Not Extended"));
	_message_status_code.insert(
			std::pair<unsigned int, std::string>(511, "Network Authentication Required"));
}

//Getters of Response class

//This method returns the status code
unsigned int Response::getStatusCode(void) const
{
	return this->_status_code;
}

//This method returns the status message
std::string Response::getStatusMessage(void) const
{
	return this->_status_message;
}

//This method returns the protocol version
std::string Response::getProtocolVersion(void) const
{
	return this->_protocol_version;
}

//This method returns body of response
std::string Response::getBody(void) const
{
	return this->_body;
}

//This method returns body length like an unsigned int 
unsigned int Response::getBodyLength(void) const
{
	return this->_body_length;
}

//This method returns the value of the header field passed like parameter
std::string Response::getHeader(std::string& header_key)
{
	std::string header_value = "";

	std::map<std::string, std::string>::iterator it = this->_headers.find(header_key);

	if(it != this->_headers.end())
		return it->second;

	return header_value;
}

//This method shows all headers and its values separated by two points and a space
void Response::showHeaders(void)
{
	std::cout<<"Headers = {"<<std::endl;
	
	for(std::map<std::string, std::string>::iterator it = this->_headers.begin();
			it != this->_headers.end(); it++)
	{
		std::cout<<it->first<<": "<<it->second<<std::endl;
	}

	std::cout<<"}"<<std::endl;
}

//Setters of Response class

//Function to set status code of response
bool Response::setStatusCode(unsigned int status_code)
{
	if(status_code >= 600)
		return false;
	this->_status_code = status_code;
	return true;
}

//Function to set status message of response
void Response::setStatusMessage(std::string status_message)
{
	this->_status_message = status_message;
}

//Function to add headers to response
bool Response::addHeader(std::string header_key, std::string header_value)
{
	unsigned int pos = 0;
	unsigned int init;

	int white_spaces = 0;

	if(header_key.find(" ") != std::string::npos)
		return false;

	//Checking whitespaces of header value
	if(header_value[pos] == ' ')
	{
		pos++;
		if(header_value[pos] == ' ')
			return false;
	}

	init = pos;
	while(pos < header_value.size())
	{
		if(header_value[pos] != ' ')
			white_spaces = 0;
		if(header_value[pos] == ' ')
			white_spaces++;
		if(white_spaces > 1)
			return false;
		pos++;
	}
	pos--;
	if(header_value[pos] == ' ')
		pos--;
	header_value = header_value.substr(init, pos - init + 1);


	this->_headers.insert(std::pair<std::string, std::string>(header_key, header_value));
	return true;
}

//Function to add a body to response
void Response::setBody(std::string& body)
{
	this->_body = body;
	this->_body_length = this->_body.size();
}
