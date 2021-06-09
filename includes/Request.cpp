#include "Request.hpp"

Request::Request() {}

Request::~Request() {}

const std::string& Request::_getMethod() const {
    return this->_method;
}

const std::string& Request::_getUri() const {
    return this->_uri;
}

const std::string& Request::_getProtocol() const {
    return this->_protocol;
}

unsigned int Request::_getContentLenght() const {
    return this->_Clen;
}

const std::string& Request::_getContentType() const {
    return this->_Ctype;
}

void Request::_parseLine(const std::string& _line)
{
    std::string _noSpace = "";

	for (size_t i = 0; i < _line.size(); i++)
	{
		if (_line[i] != ' ')
			_noSpace += _line[i];
		else
		{
			this->_parse.push_back(_noSpace);
			_noSpace = "";
		}
	}
	this->_parse.push_back(_noSpace);
}

void Request::_parseIncomingRequest(const std::string& _buffer)
{
    std::string _data;
    std::istringstream _read(_buffer);

    // std::cout << "data to parse: " << _buffer << "\n";
    while (std::getline(_read, _data))
    {
        if (_data.find("HTTP/1.1") != std::string::npos)
        {
            _parseLine(_data);
            this->_method = _parse[0];
            this->_uri = _parse[1];
            this->_protocol = _parse[2];
        }
        else if (_data.find("Content-type") != std::string::npos)
        {
            
        }
    }
    // std::cout << "Method = " << _method;
    // std::cout << "uri = " << _uri;
    // std::cout << "_protocol = " << _protocol;
}