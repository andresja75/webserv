
#include "../../inc/request/Request.hpp"

//Constructors and destructor of Request class

Request::Request(const std::string &str)
	:_method(""), _protocol(""), _url(""), _resource("")
{
	unsigned long pos;
	std::string request_line = "";
	
	pos = 0;
	//Borrar despues esto es solo para mostrar la request que llega!!!
	std::cout<<"Request: "<<std::endl;
	std::cout<<str<<std::endl;
	
	pos = str.find("\r\n");
	if(pos == std::string::npos)
		throw RequestException("No final");

	request_line = str.substr(0, pos);
	parse_request_line(request_line);
	show_headers_info();
}


//------------------- Private Methods of Request class ----------

//Method to parse the request line in a request
bool Request::parse_request_line(std::string &request_line)
{
	unsigned long len;
	std::string aux_parse_str;
	len = 0;
	
	std::cout<<"req. line: "<<request_line<<std::endl;	
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
	this->_url = request_line.substr(0, len);
	parse_url();

	request_line = request_line.substr(len + 1 , request_line.size() - 1);//Actualizamos la posicion desde la que vamos a tomar la subcadena
	
	//Parsing protocol
	this->_protocol = request_line.substr(0, request_line.size());

	return true;
}

//---------------------------------------------------------------
//------------------- Public Methods of Request class ----------

//Method to parse the url, and get the target resource and query params

bool Request::parse_url(void)
{
	std::string str = this->_url;
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
	std::cout<<"URL: "<<this->_url<<std::endl;
	std::cout<<"Resource: "<<this->_resource<<std::endl;
	std::cout<<"URL query params: {"<<std::endl;

	for(std::multimap<std::string, std::string>::iterator iter = this->_query_params.begin();
			iter != this->_query_params.end(); iter++)
	{
		std::cout<<"key: "<<iter->first<<", value: "<<iter->second<<std::endl;
	}
	std::cout<<"}"<<std::endl;
	std::cout<<"Protocol: "<<this->_protocol<<std::endl;

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
