#include "../inc/Connection.hpp"
#include <iostream>

Connection::Connection(int socket)
	: _socket(socket), send_pos(0), finish_request(false), index(-1) {
	memset(&_address, 0, sizeof(_address));
}

Connection::Connection(int socket, const std::string &max_size)
	: _socket(socket), send_pos(0), finish_request(false), index(-1) {
		_max_request_size = util::stoi(max_size);
	}

Connection::Connection(int socket, const std::string &max_size, struct sockaddr_in address)
	: _socket(socket), _address(address), send_pos(0), finish_request(false),
	index(-1) {
		_max_request_size = util::stoi(max_size);
	}

int Connection::getSocket() const { return _socket; }

const std::string &Connection::getRequest() { return _request; }

const std::string &Connection::getResponse() { return _response; }

bool Connection::isFinishRequest() const { return finish_request; }

void Connection::setResponse(std::string resp) {
	_response = resp;
}

ssize_t Connection::recv() {
	char buffer[READ_BUFFER_SIZE + 1] = {0};
	ssize_t valread = 1;

	valread = ::recv(_socket, buffer, READ_BUFFER_SIZE, 0);
	if (valread > 0 && (_max_request_size == 0 || _request.size() < (size_t)_max_request_size)) {
		_request += buffer;
	}

	if (completeRequest()) {
		if (_max_request_size && _request.size() > (size_t)_max_request_size)
			return -2;
		return 0;
	}
    return valread;
}

ssize_t Connection::send() {
	if (_response.empty()) {
		return -1;
	}

	ssize_t valsend;
	valsend = ::send(_socket, &_response.c_str()[send_pos], _response.size() - send_pos, 0);
	if (valsend > 0)
		send_pos += valsend;
	if (send_pos >= _response.size()) {
		close(_socket); // Keep connection alive ?
		return 0;
	}
    return valsend;
}

bool Connection::completeRequest() {
	if (finish_request)
		return true;
	size_t	i = _request.find("\r\n\r\n");
	if (i != std::string::npos)
	{
		if (_request.find("Content-Length: ") == std::string::npos)
		{
			if (_request.find("Transfer-Encoding: chunked") != std::string::npos)
			{
				if (_request.substr(_request.size() - 5) == "0\r\n\r\n")
					return (finish_request = true, true);
				else
					return false;
			}
			else
				return (finish_request = true, true);
		}

		ssize_t	len = util::stoi(_request.substr(_request.find("Content-Length: ") + 16,  10));
		if (_request.size() >= len + i + 4)
			return (finish_request = true, true);
		else
			return false;
	}
	return false;
}
