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

std::string Request::_getProtocol() const {
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

void Request::_pushDataToArg(std::string _data)
{
	std::string _line;
	std::istringstream _read(_data);
	ArgContent arg = {};

	while (getline(_read, _line))
	{
	// std::cout << "d = " << _data << std::endl;
		if (_line.find("Content-Type") != std::string::npos)
		{
			arg._Ctype = _line.substr(_line.find(":") + 2);
			std::cout << "c-type = " << arg._Ctype << "\n";
		}
		else if (_line.find("Content-Disposition") != std::string::npos)
		{
			arg._Cdisp = _line.substr(_line.find(":") + 2);
			std::cout << "c-disp = " << arg._Cdisp << "\n";
		}
		else if (_line.length() && _line[0] != '\r')
		{
			arg._data.append(_line);
			std::cout << "arg._data = " << arg._data << "\n";
		}
	}
}

void Request::_parseIncomingRequest(const std::string& _buffer)
{
    std::string _data;
	std::string _line;
	std::string _bdr = "";
	size_t 		f;
	std::string _buff = _buffer;
	// _buff = _buff.substr(0, _buff.find("\r"));
	ArgContent arg = {};
    std::istringstream _read(_buff);

    while (std::getline(_read, _data))
    {
		_data = _data.substr(0, _data.find("\r"));
		if (!_parse.size() && _data.find("HTTP/1.1") != std::string::npos)
		{
			_parseLine(_data);
			this->_rmap["method"] = _parse[0];
			this->_rmap["uri"] = _parse[1];
			this->_rmap["protocol"] = _parse[2];
			// this->_method = _parse[0];
			// this->_uri = _parse[1];
			// this->_protocol = _parse[2];
		}
		else if (!_rmap["Content-Type"].length() && _data.find("Content-type:") != std::string::npos)
		{
			if (_data.find("boundary=") != std::string::npos)
			{
				_line = "Content-Type";
				_rmap[_line] = _data.substr(_line.length() + 2, _data.find_first_of(';') - 14);
				_rmap["boundary"] = _bdr.append("--").append(_data.substr(_data.find("boundary=") + 9));
				// this->_Ctype = _data.substr(f + _line.length() + 1, _data.find_first_of(';') - 14);
				// this->_boundary = this->_boundary.append("--").append("boundray");
			}
			else
			{
				_rmap["form-data"] = _data.substr(_data.find(":") + 2);
				// this->_Atype = _data.substr(_data.find(":") + 2);
			}
		}
		else if (!_rmap["Host"].length() && _data.find("Host:") != std::string::npos)
		{
			_line = "Host";
			_rmap[_line] = _data.substr(_line.length() + 2
			, _data.length() - 1);
			// this->_Host = _data.substr(f + _line.length() + 1
			// , _data.length() - 1);
		}
		else if (!_rmap["Content-Length"].length() &&_data.find("Content-Length:") != std::string::npos)
		{
			_line = "Content-Length";
			std::string _slen = _data.substr(_line.length() + 2,
			_data.length() - 1);
			_rmap[_line] = _slen;
			std::stringstream ss(_slen);
			ss >> this->_Clen;
		}
		else if (!_rmap["Content-Disposition"].length() && _data.find("Content-Disposition:") != std::string::npos)
		{
			_line = "Content-Disposition";
			_rmap[_line] = _data.substr(_line.length() + 2,
			_data.length() - 1);
			// this->_Cdisp = _data.substr(f + _line.length() + 1,
			// _data.length() - 1);
		}
		else if (!_rmap["Connection"].length() && _data.find("Connection:") != std::string::npos)
		{
			_line = "Connection";
			_rmap[_line] = _data.substr(_line.length() + 2,
			_data.length() - 1);
			// this->_Conn = _data.substr(f + _line.length() + 1,
			// _data.length() - 1);
		}
		else if (!_rmap["Transfer-Encoding"].length() && _data.find("Transfer-Encoding:") != std::string::npos)
		{
			_line = "Transfer-Encoding";
			_rmap[_line] = _data.substr(_data.find(_line) + 2 + _line.length());
		}
		else if (_Clen && !_rmap["boundary"].length())
		{
			//std::cout << "bdr = " << _data << std::endl;
			if (_isArg)
			{
				_argBody.append(_data);
				arg._data = _argBody;
				std::cout << "_argBody = " << arg._data.length() << std::endl;
				if (_argBody.length() == _Clen)
				{
					arg._data = _argBody;
					_aCont.push_back(arg);
					_argBody.clear();
					_isArg = false;
				}
				else
					_argBody.append("\n");
			}
			else
				_isArg = true;
		}
		// else
		//	_pushDataToArg(_argBody);
    }
	// std::cout << "_CType = " << _Ctype << "\n";
	// std::cout << "_Clen = " << _Clen << "\n";
	// std::cout << "_Host = " << _Host << "\n";
	// std::cout << "Method = " << _method;
	// std::cout << "uri = " << _uri;
	// std::cout << "_protocol = " << _protocol;
}

std::string Request::_getHeaderContent(std::string _first)
{
	return _rmap[_first];
}

// POST / HTTP/1.1
// Host: localhost
// Transfer-Encoding: chunked

// A
// ssssssssss
// 0


// DELETE / HTTP/1.1
// Host: localhost
