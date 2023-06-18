
#include "../../inc/request/Request.hpp"

//Constructors and destructor of Request class

Request::Request(const std::string &str)
	:_method(""), _protocol(""), _path(""), _resource(""), _body("")
{
	unsigned long pos;
	std::string request_line = "";
	std::string aux_request = "";

	aux_request = str;

	pos = 0;	
	
	//This part is for request line parsing
	pos = aux_request.find("\r\n");
	if(pos == std::string::npos)
		throw RequestException("No final");

	request_line = aux_request.substr(0, pos);
	parse_request_line(request_line);

	//This part is for header fields parsing
	aux_request = aux_request.substr(pos + 2, aux_request.size());
		
	pos = aux_request.find("\r\n\r\n");
	if(pos == std::string::npos)
		throw RequestException("No final");
	aux_request = aux_request.substr(0, pos + 2);
	
	
	pos = aux_request.find("\r\n");

	while(pos != std::string::npos)
	{
		parse_and_insert_header(aux_request.substr(0, pos + 2));
		aux_request = aux_request.substr(pos + 2, aux_request.size());
		pos = aux_request.find("\r\n");

	}
	
	//Now we are going to introduce the body in the request
	aux_request = str.substr(str.find("\r\n\r\n") + 4, str.size());
	insert_body(aux_request);
}


//------------------- Private Methods of Request class ----------

//Method to parse the request line in a request
bool Request::parse_request_line(std::string &request_line)
{
	unsigned long len;
	std::string aux_parse_str;
	len = 0;
	
	//We have now the first line, the so called 'request-line'
	len = request_line.find(' ');
	if(len == std::string::npos)
		throw RequestException("No white space in format");
	this->_method = request_line.substr(0, len);

	request_line = request_line.substr(len + 1 , request_line.size() - 1);//Actualizamos la posicion desde la que vamos a tomar la subcadena
	
	//Parsing resource requested
	len = request_line.find(' ');

	if(len == std::string::npos)
		throw RequestException("No white space in format");
	this->_path = request_line.substr(0, len);
	parse_url();

	request_line = request_line.substr(len + 1 , request_line.size() - 1);//Actualizamos la posicion desde la que vamos a tomar la subcadena
	
	//Parsing protocol
	this->_protocol = request_line.substr(0, request_line.size());

	return true;
}

//Method to parse header fields, and if it is right insert them
//in a map
void Request::parse_and_insert_header(std::string header)
{
	unsigned long pos;
	std::string key = "";
	std::string value = "";

	pos = 0;
	
	pos = header.find(":");
	if(pos == std::string::npos)
		throw RequestException("No : in header fields format");
	

	key = header.substr(0, pos);

	header = header.substr(pos + 1, header.size());
	//Reset pos to 0
	pos = 0;
		
	//Checking whitespaces, it must be just 0 or 1
	if(header[pos] == ' ')
	{
		pos++;
		if(header[pos] == ' ')
			throw RequestException("Several whitespaces in header fields");
	}
	
	header = header.substr(pos, header.size());
	pos = header.find("\r\n");
	value = header.substr(0, pos);

	this->_header_fields.insert(std::pair<std::string, std::string>(key, value));
}

//Method to insert body, checking if it has content-length,
//transfer-encoding or it has no method to determine body
void Request::insert_body(const std::string &body)
{
	int num_bytes = 0;
	std::string number_of_bytes = this->getHeader("Content-Length");
	
	//Checking if it has content-length
	if(number_of_bytes != "")
	{
		num_bytes = util::stoi(number_of_bytes);
		if (num_bytes == -1)
			logger.error("Error parsing Content-Length, malformed number");
		this->_body = body.substr(0, num_bytes);
	}
	//Checking if it is transfer-encoding: chunked
	else if(this->getHeader("Transfer-Encoding") == "chunked")
	{
		this->_body = body;	
	}
	//If it is not method to determine the body
	else
	{
		this->_body = "";
	}
}

//---------------------------------------------------------------
//------------------- Public Methods of Request class ----------

//Method to parse the url, and get the target resource and query params

bool Request::parse_url(void)
{
	std::string str = this->_path;
	std::string aux = "";
	std::string key = "";
	std::string value = "";
	unsigned long len;
	unsigned long pos;
	
	len = 0;
	pos = 0;
	//Getting target resource
	len = str.find("?");
	this->_resource = str.substr(0, len);	
	//Getting query params
	str = str.substr(len + 1);
	
	len = str.find("&");
	while(len == 0)
	{
		if(str.size() == 0)
			return true;
		str = str.substr(1, str.size());
		len = str.find("&");
		if(len == std::string::npos)
			return true;
	}

	while(len != std::string::npos)
	{
		aux = str.substr(0, len);
		pos = aux.find("=");
		key = aux.substr(0, pos);
		if(pos != 0)
			value = aux.substr(pos + 1);
		this->_query_params.insert(std::pair<std::string, std::string>(key, value));
		str = str.substr(len + 1);
		len = str.find("&");
		while(len == 0)
		{
			if(str.size() == 0)
				return true;
			str = str.substr(1, str.size());
			len = str.find("&");
			if(len == std::string::npos)
				return true;
		}
		//Set default values on variables
		key = "";
		value = "";
		pos = 0;
	}

	aux = str.substr(0, len);
	pos = aux.find("=");
	key = aux.substr(0, pos);
	if(pos != 0)
		value = aux.substr(pos + 1);
	this->_query_params.insert(std::pair<std::string, std::string>(key, value));
	return true;	
}

//Method to show information about request
void Request::show_headers_info(void)
{
	std::cout<<"Parsed request: "<<std::endl;
	std::cout<<"Method: "<<this->_method<<std::endl;
	std::cout<<"URL: "<<this->_path<<std::endl;
	std::cout<<"Resource: "<<this->_resource<<std::endl;
	std::cout<<"URL query params: {"<<std::endl;

	for(std::multimap<std::string, std::string>::iterator iter = this->_query_params.begin();
			iter != this->_query_params.end(); iter++)
	{
		std::cout<<"key: "<<iter->first<<", value: "<<iter->second<<std::endl;
	}
	std::cout<<"}"<<std::endl;
	std::cout<<"Protocol: "<<this->_protocol<<std::endl;

	std::cout<<"Header-fields: {"<<std::endl;
	for(std::map<std::string, std::string>::iterator it = this->_header_fields.begin();
			it != this->_header_fields.end(); it++)
	{
		std::cout<<it->first<<": "<<it->second<<std::endl;
	}
	std::cout<<"}"<<std::endl;

	std::cout<<"Body:"<<std::endl;
	std::cout<<this->_body<<std::endl;


}

//Method to give the target file specified like parameter in function,
//is case-sensitive
std::string Request::getHeader(std::string header)
{
	std::map<std::string, std::string>::iterator it = this->_header_fields.find(header);

	if(it == this->_header_fields.end())
		return "";

	return it->second;
}

//Method to get the verb used in the request
std::string Request::getMethod(void) const
{	
	return (this->_method);
}

//Method to get the complete url used in the request
std::string Request::getPath(void) const
{	
	return (this->_path);
}

//Method to get the target resource in the request
std::string Request::getResource(void) const
{
	return (this->_resource);
}

//Method to get the protocol used in the request
std::string Request::getProtocol(void) const
{	
	return (this->_protocol);
}

//Method to get the body used in the request
std::string Request::getBody(void) const
{
	return (this->_body);
}

//---------------------------------------------------------------
//------------------- Class RequestException --------------------
//Constructors and destructor of Request Exception

Request::RequestException::RequestException(const std::string &msg)
	: _msg("RequestException: " + msg)
{}

Request::RequestException::RequestException(void)
	: _msg("RequestException: ")
{}

Request::RequestException::~RequestException() throw()
{}

//Methods of RequestException error class
const char * Request::RequestException::what() const throw()
{
	return (this->_msg.c_str());
}
