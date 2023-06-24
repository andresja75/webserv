
#include "../../inc/response/Response.hpp"

std::map<unsigned int, std::string> Response::_message_status_code = init_message_status_code();
//Constructors and destructor of Response class

//Response constructor with unsigned int constructor
Response::Response(unsigned int status_code)
	:_status_code(status_code), _protocol_version(HTTP_PROTOCOL),
	_body(""), _body_length(0) 
{
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
std::map<unsigned int, std::string> Response::init_message_status_code(void)
{
	std::map<unsigned int, std::string> message_status_code;
	//100 status codes
	message_status_code.insert(
			std::pair<unsigned int, std::string>(100, "Continue"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(101, "Switching protocol"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(102, "Processing"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(103, "Early Hints"));

	//200 status codes
	message_status_code.insert(
			std::pair<unsigned int, std::string>(200, "Ok"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(201, "Created"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(202, "Accepted"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(203, "Non-Authorative Information"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(204, "No Content"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(205, "Reset Content"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(206, "Partial Content"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(207, "Multi-Status"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(208, "Multi-Status"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(226, "IM Used"));
	
	//300 status codes
	message_status_code.insert(
			std::pair<unsigned int, std::string>(300, "Multiple Choice"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(301, "Moved Permanently"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(302, "Found"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(303, "See Other"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(304, "Not Modified"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(305, "Use Proxy"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(307, "Temporary Redirect"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(308, "Permanent Redirect"));

	//400 status codes	
	message_status_code.insert(
			std::pair<unsigned int, std::string>(400, "Bad Request"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(401, "Unauthorized"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(403, "Forbidden"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(404, "Not Found"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(405, "Method Not Allowed"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(406, "Not Acceptable"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(407, "Proxy Authentication Required"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(408, "Request Timeout"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(409, "Conflict"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(410, "Gone"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(411, "Length Required"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(412, "Precondition Failed"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(413, "Payload Too Large"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(414, "URI Too Long"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(415, "Unsupported Media Type"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(416, "Requested Range Not Satisfiable"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(417, "Expectation Failed"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(418, "I'm a teapot"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(421, "Misredirect Request"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(422, "Unprocessable Entity"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(423, "Locked"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(424, "Failed Dependency"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(426, "Upgrade Required"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(428, "Precondition Required"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(429, "Too Many Requests"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(431, "Request Header Fields Too Large"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(451, "Unavailable For Legal Reasons"));

	//500 status codes	
	message_status_code.insert(
			std::pair<unsigned int, std::string>(500, "Internal Server Error"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(501, "Not Implemented"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(502, "Bad Gateway"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(503, "Service Unavailable"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(504, "Gateway Timeout"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(505, "HTTP Version Not Supported"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(506, "Variant Also Negotiates"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(507, "Insufficient Storage"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(508, "Loop Detected"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(510, "Not Extended"));
	message_status_code.insert(
			std::pair<unsigned int, std::string>(511, "Network Authentication Required"));
	return message_status_code;
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
	addHeader("Content-Length", util::itos(this->_body_length));
}

std::string Response::toString() {
	std::stringstream ss;
	ss << getProtocolVersion() << " " << getStatusCode() << " " << getStatusMessage() << "\r\n";
	for (std::map<std::string, std::string>::iterator it = _headers.begin();
		it != _headers.end(); it++) {
		ss << it->first << ": " << it->second << "\r\n";
	}
	ss << "\r\n";
	if (_body.size()) {
		ss << getBody();
		ss << "\r\n";
	}
	return ss.str();
}
