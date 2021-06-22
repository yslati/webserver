#include "Request.hpp"
#include <regex>

Request::Request()
{
	_Clen = 0;
	_isArg = false;
	_boundary = "";
	_error = 0;
	_isDone = false;
	_postBody = "";
	_isLine = false;
	_isL = false;
}

Request::~Request()
{
	_body.clear();
	_aCont.clear();
}

int		Request::_getError()
{
	return _error;
}

int		Request::_getContentLen()
{
	return (_Clen);
}

std::string Request::_getPostBody() const
{
	return _postBody;
}

std::string		Request::_getBoundary() const
{
	return _boundary;
}

unsigned int Request::_getPostLenght(std::string data, std::string boundary)
{
	std::string _line = "";
	std::string body = "";
	std::istringstream _read(data);
	bool _isEmpty = true;

	while (getline(_read, _line))
	{
		if (_isEmpty)
			body.append(_line).append("\n");
	}
	if (!boundary.length())
		body.pop_back();
    return body.length();
}

// unsigned int Request::_getPostLenght(std::string data, std::string boundary)
// {
// 	std::string _line = "";
// 	std::string body = "";
// 	std::istringstream _read(data);
// 	bool _isEmpty = true;

// 	while (getline(_read, _line))
// 	{
// 		if(_isEmpty)
// 			body.append(_line).append("\n");
// 	}
// 	if (!boundary.length())
// 		body.pop_back();
//     return body.length();
// }

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
	std::istringstream _read(_data);
	ArgContent arg = {};

	bool is_info = false;
	while (getline(_read, _line))
	{
		if (_line.find("Content-Type") != std::string::npos)
			arg._Ctype = _line.substr(_line.find(":") + 2);
		else if (_line.find("Content-Disposition") != std::string::npos)
		{
			arg._Cdisp = _line.substr(_line.find(":") + 2);
			is_info = true;
		}
		else
		{
			if (is_info)
			{
				is_info = false;
				continue;
			}
			arg._data.append(_line).append("\r\n");
		}
	}
	return (arg);
}

void	Request::_pushDataToArg(std::string _data)
{
	std::string _regex = "--" + _boundary;

	if (_regex.length() && _matchBegin(_regex, _data))
	{
		if (!_isArg)
			_isArg = true;
		_aCont.push_back(_pushToArg(_body));
		_body.clear();
	}
	else if (_isArg)
		_body.append(_data).append("\n");
}

void Request::_parseQueryString(std::string uri)
{
	if (uri.find("?") != std::string::npos)
		_rmap["query_string"] = uri.substr(uri.find("?") + 1, uri.length());
}

void Request::_pushToPostBody(std::string _data)
{
	std::istringstream _read(_data);
	std::string _line = "";

	while (getline(_read, _line))
	{
		if (_isLine)
		{
			if (_line.length())
				_postBody.append(_line).append("\n");
		}
		else
			_isLine = true;
	}
}

void Request::_parseIncomingRequest(const std::string& _buffer)
{
    std::string _data;
	std::string _line;
	std::string _bdr = "";
	size_t 		f;
	std::string _buff; 
	_buff.append(_buffer);
	ArgContent arg = {};
    std::istringstream _read(_buff);


    while (std::getline(_read, _data))
    {
		if (!_parse.size() && _data.find("HTTP/1.1") != std::string::npos)
		{
			_parseLine(_data);
			this->_rmap["method"] = _parse[0];
			this->_rmap["uri"] = _parse[1];
			_parseQueryString(_rmap["uri"]);
			this->_rmap["protocol"] = _parse[2];
			this->_rmap["protocol"].pop_back();
		}
		else if (!_rmap["Content-Type"].length() && _data.find("Content-Type:") != std::string::npos)
		{
			if (_data.find("boundary=") != std::string::npos)
			{
				_line = "Content-Type";
				// _rmap[_line] = _data.substr(_line.length() + 2, _data.find_first_of(';') - 14);
				_rmap[_line] = _data.substr(_line.length() + 2);
				_rmap[_line].pop_back();
				// std::cout << "CC = " << _rmap[_line] << std::endl;
				_rmap["boundary"] = _bdr.append("--").append(_data.substr(_data.find("boundary=") + 9));
				_rmap["boundary"].pop_back();
				_boundary = _data.substr(_data.find("boundary=") + 9);
				_boundary.pop_back();
			}
			else
			{
				_rmap["Content-Type"] = _data.substr(_data.find(":") + 2);
				_rmap["Content-Type"].pop_back();
				// std::cout << "TT = " << _rmap["Content-Type"] << std::endl;
			}
		}
		else if (!_rmap["Host"].length() && _data.find("Host:") != std::string::npos)
		{
			_line = "Host";
			_rmap[_line] = _data.substr(_line.length() + 2
			, _data.length() - 1);
			_rmap["port"] = _rmap[_line].substr(_rmap[_line].find(":") + 1,
			_rmap[_line].length() - 1);
			_rmap["port"].pop_back();
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
			std::cout << "hnaya = " << _rmap[_line] << "\n";
		}
		else if (!_rmap["Connection"].length() && _data.find("Connection:") != std::string::npos)
		{
			_line = "Connection";
			_rmap[_line] = _data.substr(_line.length() + 2,
			_data.length() - 1);
			_rmap[_line].pop_back();
		}
		else if (!_rmap["Transfer-Encoding"].length() && _data.find("Transfer-Encoding:") != std::string::npos)
		{
			_line = "Transfer-Encoding";
			_rmap[_line] = _data.substr(_data.find(_line) + 2 + _line.length());
		}
		else if (_Clen && !_rmap["boundary"].length())
		{
			if (_isL)
				_postBody.append(_data);
			else
				_isL = true;
			if (_isArg)
			{
				// _argBody.append(_data);
				if (_argBody.length())
				{
					arg._data = _argBody;
					_argBody.clear();
					_aCont.push_back(arg);
					_isArg = false;
				}
				else
					_argBody.append("\n");
			}
			else
				_isArg = true;
		}
		else if (_rmap["Content-Length"].length())
		{
			// std::cout << "dt1 = " << _data << "\n";
			_pushToPostBody(_data);
			_pushDataToArg(_data);
		}
    }
	// std::cout << "================================================\n";
	// std::cout << _postBody << std::endl;
	// std::cout << _getPostLenght(_postBody, _boundary) << std::endl;
	// std::cout << _Clen << std::endl;
	// std::cout << "================================================\n";
	// if (!_boundary.length() || (_Clen && _Clen == _getPostLenght(_postBody, _boundary)))
	// 	_isDone = true;
	// if (_isDone)
	// {
	// 	if (_rmap["method"].compare("GET") && _rmap["method"].compare("POST") &&
	// 	_rmap["method"].compare("DELETE"))
	// 		_error = 1;
	// 	else if (!_rmap["uri"].length())
	// 		_error = 1;
	// 	else if (!_rmap["Host"].length())
	// 		_error = 1;
	// }
}

Request::ArgContent Request::_getArg(size_t i)
{
	return _aCont[i];
}

std::string Request::_getCgiUriFile()
{
	return _rmap["uri"];
}

std::string Request::_getHeaderContent(std::string _first)
{
	return _rmap[_first];
}

std::vector<Request::ArgContent> Request::_getVecCont() const
{
	return _aCont;
}