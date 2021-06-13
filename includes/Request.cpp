#include "Request.hpp"
#include <regex>

Request::Request()
{
	this->_Host = "";
	this->_method = "";
	this->_uri = "";
	this->_protocol = "";
	this->_Server = "";
	this->_Ctype = "";
	this->_Atype = "";
	this->_Cdisp = "";
	this->_Clen = 0;
	this->_Conn = "";
	_lenArg = 0;
	_isArg = false;
	_boundary = "";
}

Request::~Request()
{
	this->_Host = "";
	this->_method = "";
	this->_uri = "";
	this->_protocol = "";
	this->_Server = "";
	this->_Ctype = "";
	this->_Atype = "";
	this->_Cdisp = "";
	this->_Clen = 0;
	this->_Conn = "";
	_body = "";
	_aCont.clear();
}

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

bool Request::_isPrefix(std::string& s1, std::string& s2)
{
	size_t n1 = s1.length();
	size_t n2 = s2.length();
	if (!n1 || !n2)
		return false;
	for (int i = 0; i < n1; i++)
	{
		if (s1[i] != s2[i])
		{
			if (i + 1 == n1)
				return true;
			return false;
		}
	}
	return true;
}

bool Request::_matchBegin(std::string& _regex, std::string& _line)
{
	std::string _r = _regex;
	_r.pop_back();

	return _line.compare(0, _r.size(), _r) == 0;
}

void	Request::_printArg()
{
	std::cout << "s = " << _aCont.size() << "\n";
	for (int i = 0; i < _aCont.size(); i++)
	{
		ArgContent arg = _aCont[i];
		std::cout << "i = " << i << " _Cdisp = " << arg._Cdisp
		<< " _Ctype = " << arg._Ctype << " _data = " << arg._data << std::endl;
	}
}

Request::ArgContent Request::_pushToArg(std::string _data)
{

	std::string _line;
	// _data.pop_back();
	std::istringstream _read(_data);
	ArgContent arg = {};

	bool is_info = false;
	while (getline(_read, _line))
	{
		// std::smatch match;
		// std::regex re("boundary");

		// std::regex_search(_line, match, re);
		// _line.pop_back();
		if (!_line.length())
			continue;
		if (_line.find("Content-Type") != std::string::npos)
			arg._Ctype = _line.substr(_line.find(":") + 2);
		else if (_line.find("Content-Disposition") != std::string::npos)
		{
			arg._Cdisp = _line.substr(_line.find(":") + 2);
			is_info = true;
		}
		else
		{
			if (is_info) {
				is_info = false;
				continue;
			}
			// if (arg._data.length())
			// 	arg._data += "\n";
			arg._data.append(_line);
		}
	}
	/*if (_isArg)
	{
		_aCont.push_back(arg);
		_isArg = false;
		_lenArg++;
	}*/
	return (arg);
}

void	Request::_pushDataToArg(std::string _data)
{
	std::string _regex = "--" + _boundary;
	// std::cout << "_match = " << _matchBegin(_regex, _data) << std::endl;
	// std::cout << "c = " << _data << std::endl;
	// std::cout << "r = " << _regex << std::endl;

	// _data.pop_back();
	if (_regex.length() && _matchBegin(_regex, _data))
	{
		if (!_isArg)
			_isArg = true;
		_aCont.push_back(_pushToArg(_body));
		_body.clear();
	}
	else if (_isArg)
		_body.append(_data).append("\n");
	// std::cout << "_body = " << _body << std::endl;
	// else
		// _isArg = true;
}

void Request::_parseIncomingRequest(const std::string& _buffer)
{
    std::string _data;
	std::string _line;
	std::string _bdr = "";
	size_t 		f;
	std::string _buff; 
	_buff.append(_buffer);
	// _buff = _buff.substr(0, _buff.find("\r"));
	ArgContent arg = {};
    std::istringstream _read(_buff);
	// std::cout << "start--------------------------------------" << std::endl;

    while (std::getline(_read, _data))
    {
		// _data = _data.substr(0, _data.find("\r"));
		// std::cout << "current line = " << _data << std::endl;
		if (!_parse.size() && _data.find("HTTP/1.1") != std::string::npos)
		{
			_parseLine(_data);
			this->_rmap["method"] = _parse[0];
			this->_rmap["uri"] = _parse[1];
			this->_rmap["protocol"] = _parse[2];
			// this->_rmap["method"].pop_back();
			// this->_rmap["uri"].pop_back();
			this->_rmap["protocol"].pop_back();
			// this->_method = _parse[0];
			// this->_uri = _parse[1];
			// this->_protocol = _parse[2];
		}
		else if (!_rmap["Content-Type"].length() && _data.find("Content-Type:") != std::string::npos)
		{
			if (_data.find("boundary=") != std::string::npos)
			{
				_line = "Content-Type";
				_rmap[_line] = _data.substr(_line.length() + 2, _data.find_first_of(';') - 14);
				_rmap["boundary"] = _bdr.append("--").append(_data.substr(_data.find("boundary=") + 9));
				_rmap["boundary"].pop_back();
				// this->_Ctype = _data.substr(f + _line.length() + 1, _data.find_first_of(';') - 14);
				_boundary = _data.substr(_data.find("boundary=") + 9);
				_boundary.pop_back();
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
		else if (!_rmap["Content-Length"].length() && _data.find("Content-Length:") != std::string::npos)
		{
			_line = "Content-Length";
			std::string _slen = _data.substr(_line.length() + 2,
			_data.length() - 1);
			_rmap[_line] = _slen;
			std::stringstream ss(_slen);
			ss >> this->_Clen;
		}
		else if (!_rmap["Content-Length"].length() && !_rmap["Content-Disposition"].length() &&
		_data.find("Content-Disposition:") != std::string::npos)
		{
			_line = "Content-Disposition";
			_rmap[_line] = _data.substr(_line.length() + 2,
			_data.length() - 1);
			// this->_Cdisp = _data.substr(f + _line.length() + 1,
			// _data.length() - 1);
			std::cout << "hnaya = " << _rmap[_line] << "\n";
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
		// else if (_Clen && !_rmap["boundary"].length())
		// {
		// 	std::cout << "bdr = " << _data << std::endl;
		// 	if (_isArg)
		// 	{
		// 		_argBody.append(_data);
		// 		arg._data = _argBody;
		// 		std::cout << "_argBody = " << arg._data.length() << std::endl;
		// 		if (_argBody.length() == _Clen)
		// 		{
		// 			arg._data = _argBody;
		// 			_aCont.push_back(arg);
		// 			_argBody.clear();
		// 			_isArg = false;
		// 		}
		// 		else
		// 			_argBody.append("\n");
		// 	}
		// 	else
		// 		_isArg = true;
		// }
		else if (_rmap["Content-Length"].length())
		{
			// std::cout << "d = " << _data << std::endl;
			// _data = _data.substr(0, _data.find("\r"));
			// _aCont.push_back(_pushDataToArg(_data));
			// _argBody.append(_data);
			// if (_isArg)
			// {
			// 	// arg._data = _argBody;
			// 	if (_argBody.length() == _rmap["Content-Length"].length())
			// 	{
			// 		arg._data = _argBody;
			// 		_aCont.push_back(arg);
			// 		_argBody.clear();
			// 		_isArg = false;
			// 	}
			// 	else
			// 		_argBody.append("\n");
			// }
			// else
			// 	_isArg = false;
			// _argBody.append("\n");
			_pushDataToArg(_data);
			// _lenArg++;
		}
    }
	// std::cout << "START" << std::endl;
	// std::cout << _buffer << std::endl;
	// std::cout << "END" << std::endl;
	// std::cout << "len = " << _lenArg << "\n";
	// _printArg();
	// for (std::vector<Request::ArgContent>::iterator it = _aCont.begin();
	// it != _aCont.end(); it++)
	// {
	// 	Request::ArgContent arg = *it;
	// 	std::cout << arg._Ctype << std::endl;
	// 	std::cout << arg._Cdisp << std::endl;
	// 	std::cout << arg._data << std::endl;
	// }
	// std::cout << "_CType = " << _Ctype << "\n";
	// std::cout << "_Clen = " << _Clen << "\n";
	// std::cout << "_Host = " << _Host << "\n";
	// std::cout << "Method = " << _method;
	// std::cout << "uri = " << _uri;
	// std::cout << "_protocol = " << _protocol;
}

Request::ArgContent Request::_getArg(size_t i)
{
	return _aCont[i];
}


std::string Request::_getHeaderContent(std::string _first)
{
	return _rmap[_first];
}

std::vector<Request::ArgContent> Request::_getVecCont() const
{
	return _aCont;
}

// POST / HTTP/1.1
// Host: localhost
// Transfer-Encoding: chunked

// A
// ssssssssss
// 0


// DELETE / HTTP/1.1
// Host: localhost
