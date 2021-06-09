#include "Request.hpp"

Request::Request()
{
	this->_Host = "";
	this->_method = "";
	this->_uri = "";
	this->_protocol = "";
	this->_Server = "";
	this->_Date = "";
	this->_Ctype = "";
	this->_Atype = "";
	this->_Cdisp = "";
	this->_Clen = 0;
	this->_Lmodified = "";
	this->_Conn = "";
	this->_Etag = "";
	this->_Aranges = "";
	this->_boundary = "";
}

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
	std::string _line;
	size_t 		f;
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
        else if ((f = _data.find("Content-type:")) != std::string::npos)
        {
			if (!this->_boundary.length())
			{
				_line = "Content-type:";
				this->_Ctype = _data.substr(f + _line.length() + 1
				, _data.find_first_of(';') - 14);
				this->_boundary = this->_boundary.append("--").append()
			}
			else
				this->_Atype = _data.substr(_data.find(":") + 2);
		}
		else if ((f = _data.find("Host:")) != std::string::npos)
		{
			_line = "Host:";
			this->_Host = _data.substr(f + _line.length() + 1
			, _data.length() - 1);
		}
		else if ((f = _data.find("Content-Length:")) != std::string::npos)
		{
			_line = "Content-Length:";
			std::string _slen = _data.substr(f + _line.length() + 1,
			_data.length() - 1);
			std::stringstream ss(_slen);
			ss >> this->_Clen;
		}
		else if ((f = _data.find("Content-Disposition:")) != std::string::npos)
		{
			_line = "Content-Disposition:";
			this->_Cdisp = _data.substr(f + _line.length() + 1,
			_data.length() - 1);
		}
		else if ((f = _data.find("Connection:")) != std::string::npos)
		{
			_line = "Connection:";
			this->_Conn = _data.substr(f + _line.length() + 1,
			_data.length() - 1);
		}
		else if ((f = _data.find("we'll see 4eda")) != std::string::npos)
		{
			_line = "we'll see 4eda";
		}
    }
	std::cout << "_CType = " << _Ctype << "\n";
	std::cout << "_Clen = " << _Clen << "\n";
	std::cout << "_Host = " << _Host << "\n";
	// std::cout << "Method = " << _method;
	// std::cout << "uri = " << _uri;
	// std::cout << "_protocol = " << _protocol;
}