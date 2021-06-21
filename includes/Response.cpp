#include "Response.hpp"
#include <regex>

Response::Response(Location& location, HttpServer& httpServ): _location(location), _httpServ(httpServ)
{
	// std::cout << "wa si = " << _location.getRoot() << std::endl;
    _body = "";
    _ResponseContent = "";
    _status = 0;
}

Response& Response::operator=(Response const & rhs)
{
	if (this != &rhs)
	{
		_location = rhs._location;
		_httpServ = rhs._httpServ;
	}
	return *this;
}

Response::Response(Response const & rhs) : _location(rhs._location),  _httpServ(rhs._httpServ)
{
	*this = rhs;
}

Response::~Response()
{
    _body.clear();
    _ResponseContent.clear();
	_stResp.clear();
}

void Response::_setRequest(Request& req)
{
    _request = req;
}

void Response::_setLocation(Location& location)
{
    _location = location;
}

Location Response::_getLocation() const
{
    return _location;
}

int		Response::_isCGI()
{
	return (0);
}

void	Response::_handleCGI()
{
}

std::string Response::_getDir(void)
{
    char buff[1024];
    std::string dir = "";
	// std::vector<HttpServer>::iterator it = _request._getIterator();

    if (!getcwd(buff, sizeof(buff)))
        std::cerr << "getcwd failed" << std::endl;
    else
	{
        dir = std::string(buff);
		// if there is a location path
		// std::cout << "hnaya = " << _location.getRoot() << "\n";
		if (_location.getRoot().length())
		{
			if (_location.getRoot().front() != '/')
				dir += "/";
			dir.append(_location.getRoot());
		}
		else
		{
			std::cout << "rootDefault = " <<
			_httpServ.getRoot() << "\n";
			if (_httpServ.getRoot().length() &&
			_httpServ.getRoot().front() != '/')
				dir += "/";
			dir.append(_httpServ.getRoot());
		}
	}
    return (dir);
}

std::string Response::_getUploadDir()
{
	std::string dir = _getDir();
	if (dir.back() != '/')
		dir.append("/");
	std::string _uploadDir = dir.append(_location.getUploadDir());
	if (_uploadDir.back() != '/')
		_uploadDir.append("/");

	return (_uploadDir);
}

std::string Response::_getFilePath(std::string uri)
{
    std::string path = ""; 
    path = _getDir();
    path.append(uri);
    return (path);
}

std::string Response::_getFileNameFromUri(std::string uri)
{
	// Parse filename from uri
	std::string filename;
	std::string u = uri;
	int _exist = 0;
	if (uri.find("?") != std::string::npos)
		filename = uri.substr(0, uri.find("?"));
	else
	{
		if (uri.back() != '/')
			uri += "/";
		filename = uri;
	}
	// then check if filename match any of locations spicified in the config file
	if (_location.getUri().compare(filename) == 0)
		// if (filename.append(_location.getIndex()) == is_exist)
		_exist = 1;
	// if true break and return the filename
	// if (_exist)
		// return (filename);
	// std::cout << "index = " << _location.getIndex() << std::endl;
	if (_exist)
    	return (filename.append(_location.getIndex()));
	return (u);
}

bool	Response::_isSuffix(std::string s1, std::string s2)
{
    int n1 = s1.length(), n2 = s2.length();
    if (n1 > n2)
      return false;
    for (int i=0; i<n1; i++)
       if (s1[n1 - i - 1] != s2[n2 - i - 1])
           return false;
    return true;
}

void	Response::_readErrorPageFile(std::string file)
{
	std::string _line;
	std::string body = "";
	std::ifstream input_file(file);

	while (getline(input_file, _line))
		body.append(_line).append("\n");
	_body = body.append("\r\n");
}

void Response::_readFile(std::string file)
{
	std::string _line;
	std::string body = "";
	std::ifstream input_file(file);

	if (input_file.fail())
	{
		_status = S_NOT_FOUND;
		if (_httpServ._getErrorPages(_status).length())
			_readErrorPageFile(_httpServ._getErrorPages(_status));
		else
			_generateErrorPage();
		return ;
	}
	while (getline(input_file, _line))
		body.append(_line).append("\n");
	_body = body.append("\r\n");
	_status = S_OK;
}

void Response::_applyGetMethod()
{
    std::string path = _getFilePath(_getFileNameFromUri(_request._getHeaderContent("uri")));
	// if (_checkPermission(path, R_OK))
	//	_handleError();
	// else
    //	_readFile(path);
	// if (DEBUG_MODE)
	// 	path.append("file.html");
	// path = "/dir/";
	// if (path.back() == '/')
	// 	path.pop_back();
	// std::cout << "path1 = " << path << "\n";
	// std::cout << "uri = " << _request._getHeaderContent("uri").substr(1, _request._getHeaderContent("uri").length()) << "\n";
	// std::cout << "uri1 = " << _location.getIndex() << "\n";
	// if (_location.getIndex().length() &&
	// _request._getHeaderContent("uri").substr(1, _request._getHeaderContent("uri").length()).compare(_location.getIndex()))
	// {
	// 	if (path.back() != '/')
	// 		path.append("/");
	// 	path.append(_location.getIndex());
	// }
	std::cout << "path = " << path << std::endl;
	if (_isDir(path))
	{
		std::cout  << "path1 = " << path << "\n";
		std::cout  << "bool = " << _location.getAutoIndex() << "\n";
		if (_location.getAutoIndex())
		{
			_applyAutoIndexing(path);
			_status = S_OK;
		}
		else
			_status = S_NOT_FOUND;
	}
	else if (!_checkPermission(path, R_OK))
		_readFile(path);
	// std::cout << _body << std::endl;
}

std::string Response::_getFileNameFromDisp(std::string disp)
{
    std::string path = "";
    std::regex re("filename=\"");
    std::smatch match;
    if (disp.length())
    {
        std::regex_search(disp, match, re);
        if (!match.empty())
        {
            path = match.suffix();
            path = path.substr(0, path.find("\""));
        }
    }
	return path;
}

void Response::_applyPostMethod()
{
    std::string _filename = "";
    std::string _dir = "";
    std::string _line = "";
    // get the path of the file
    // put the data of the post method in the file
    for (size_t i = 0; i < _request._getVecCont().size(); i++)
    {
        Request::ArgContent arg = _request._getArg(i);
        _filename = _getFileNameFromDisp(arg._Cdisp);

        if (_filename.length())
        {
			// _dir = _getDir().append("/").append(_filename);
			if (_location.getIsUploadEnable())
			{
				_dir = _getUploadDir();
				_dir.append(_filename);
			}
			std::cout << "upload_dir = " << _dir << "\n";
			std::ofstream _file(_dir);
			// std::cout << arg._data << "\n";
			// _file << arg._data;
			std::istringstream ss(arg._data);

			while (getline(ss, _line))
				_file << _line.substr(0, _line.find("\r")).append("\n");
			_file.close();
		}
	}
	_body = "<html>\r\n";
	_body += "<body>\r\n";
	_body += "<h1>File uploaded.</h1>\r\n";
	_body += "</body>\r\n";
	_body += "</html>\r\n";
	_status = S_OK;
}

int Response::_checkPermission(std::string path, int mode)
{
    int retval = access(path.c_str(), mode);
    if (retval != 0)
    {
        if (errno == EACCES)
            return (1);
    }
    return (0);
}

void	Response::_generateErrorPage()
{
	_body = "<html>\r\n";
	_body += "<head>\r\n";
	_body += "\t<meta charset=\"UTF-8\">\r\n";
	_body += "\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\r\n";
	_body += "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n";
	_body += "\t<title>$1</title>\r\n";
	_body += "</head>\r\n";
	_body += "<body>\r\n";
	_body += "\t<center>\r\n";
	_body += "\t\t<h1>";
	_body += "$1";
	_body += "</h1>\r\n";
	_body += "\t</center>\r\n";
	_body += "	<hr>\r\n";
	_body += "\t<center>webserv/0.0</center>\r\n";
	_body += "</body>\r\n";
	_body += "</html>\r\n\r\n";
	_body.replace(_body.find("$1"), 2, _stResp[_status]);
	_body.replace(_body.find("$1"), 2, _stResp[_status]);
}

void Response::_deleteFile(std::string _file)
{
	// if (_checkPermission(_file, W_OK))
	// 	throw Response::PermissionDiend();
	std::cout << "_file = " << _file << std::endl;
	if (std::remove(_file.c_str()) == 0)
	{
		_body = "<html>\r\n";
		_body += "	<body>\r\n";
		_body += "		<h1>File deleted.</h1>\r\n";
		_body += "	</body>\r\n";
		_body += "</html>\r\n";
		_status = S_OK;
	}
	else
	{
		_status = S_METHOD_NOT_ALLOWED;
		std::cout << "file not found\n";
	}
}

void Response::_applyDeleteMethod()
{
    std::string path = _getFilePath(_request._getHeaderContent("uri"));

	if (_isDir(path))
	{
		_status = S_FORBIDDEN;
		std::cout << "path_err = " << _httpServ._getErrorPages(_status) << "\n";
		if (_httpServ._getErrorPages(_status).length())
		{
			_readFile(_httpServ._getErrorPages(_status));
			std::cout << _body << std::endl;
		}
		else
			_generateErrorPage();
	}
	else
		_deleteFile(path);
	
}


std::string Response::_generateHtmlTemplate()
{
	return "<!DOCTYPE html>\r\n\
<html lang=\"en\">\r\n\
<head>\r\n\
    <meta charset=\"UTF-8\">\r\n\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n\
    <title>WebServer</title>\r\n\
</head>\r\n\
<body>\r\n\
		$2\r\n\
        $1\r\n\
</body>\r\n\
</html>\r\n\r\n";
}

std::string Response::_getHrefLink(std::string dirname)
{
	std::stringstream ss;

	ss << "<p><a href=\"" + dirname + "\">" + dirname + "</p></a>\n\t\t";
	return ss.str();
}

int Response::_isDir(std::string dirname)
{
	// std::string dir = _getDir();
	// if (dirname.front() != '/')
	// 	dir += "/";
	// dir += dirname;
	// std::cout << "dir = " << dir << "\n";
	if (!(opendir(dirname.c_str())))
		return 0;
	return 1;
}

void	Response::_applyAutoIndexing(std::string _dir)
{
	DIR *dir;
	struct dirent *pDirent;
	std::string _line = "";
	std::string _name = "";
	std::string _uri = "/";
	std::string _data = "";
	// std::string _dir = "/Users/aaqlzim/Desktop/webserv/webserver/website/dir";
	std::string _index = "<h1>Index of: " + _uri + "</h1>\n\t\t<hr/>";

	_body.append(_generateHtmlTemplate());
	_body.replace(_body.find("$2"), 2, _index);
	if ((dir = opendir(_dir.c_str())))
	{
		while ((pDirent = readdir(dir)))
		{
			_name = pDirent->d_name;
			if (pDirent->d_type == DT_DIR)
				_name += "/";
			_data.append(_getHrefLink(_name));
		}
	}
	_body.replace(_body.find("$1"), 2, _data);

	if (DEBUG_MODE)
	{
		std::ofstream file("test.html");
		std::istringstream ss(_body);

		while (getline(ss, _line))
		{
			file << _line;
			file << std::endl;
		}
	}
	// std::cout << _body << std::endl;
}

bool	Response::_matchBegin(std::string _regex, std::string _line)
{
	std::string _r = _regex;
	_r.pop_back();

	return _line.compare(0, _r.size(), _r) == 0;
}

int		Response::_checkAllowedMethod(std::string method)
{
	std::vector<std::string> v = _location.getAllowedMethod();

	for (size_t i = 0; i < v.size(); i++)
	{
		if (method.compare(v[i]) == 0)
			return (1);
	}
	return (0);
}

void	Response::_handleRedirect()
{
	_status = S_MOVED_PERM;
}

void	Response::_applyMethod()
{
    // std::vector<HttpServer>::iterator it = _request._getIterator();
    // std::vector<Location> locations = it->getLocations();
    // std::vector<Location>::iterator lit = locations.begin();
    // for (; lit != locations.end(); lit++)
    // {
    //     // if the uri match one of the paths in location
    //     // setLocation(*lit);
    //     if (_matchBegin(_request._getHeaderContent("uri"), _location.getUri()))
	// 	{
    //         _setLocation(*lit);
	// 	}
	// 	else if (_location.getUri().length() < _request._getHeaderContent("uri").length())
	// 	{
	// 		// (*lit).checkVal();
	// 		_setLocation(*lit);
	// 	}
        // if (_request._getHeaderContent("uri").compare(lit->getUri()) == 0)
		// {
		// 	std::cout << "uri = " << _request._getHeaderContent("uri") << "\n";
		// 	std::cout << "_location.uri = " << lit->getUri() << "\n";
		// }
    // }
	// std::cout << "method = " << _request._getHeaderContent("method") << "\n";
	// std::cout << "_bool = " << _checkAllowedMethod("DELETE") << "\n";
	// std::cout << "uri = " << _getLocation().getUri() << std::endl;
	// std::cout << "my_uri = " << _request._getHeaderContent("uri") << std::endl;
	if (_request._getError())
		_status = S_BAD_REQ;
	else if (_httpServ.getMaxBodySize() != -1
	&& _request._getContentLen() > _httpServ.getMaxBodySize() * 1024 * 1024)
		_status = S_PAY_LOAD_TOO_LARGE;
	else if (_isCGI())
		_handleCGI();
	else if (_request._getHeaderContent("method").compare("GET") == 0
	&& _checkAllowedMethod("GET"))
	{
		if (_location.getIsRedirect())
			_handleRedirect();
		else
			_applyGetMethod();
	}
	else if (_request._getHeaderContent("method").compare("POST") == 0
	&& _checkAllowedMethod("POST"))
		_applyPostMethod();
	else if (_request._getHeaderContent("method").compare("DELETE") == 0
	&& _checkAllowedMethod("DELETE"))
		_applyDeleteMethod();
	else
		_status = S_NOT_IMPLEMENTED;
}

void Response::_makeStatus()
{
    _stResp[S_OK] = "OK";
    _stResp[S_MOVED_PERM] = "Moved Permanently";
    _stResp[S_TEMP_REDIR] = "Temporary Redirect";
    _stResp[S_BAD_REQ] = "Bad Request";
    _stResp[S_FORBIDDEN] = "Forbidden";
    _stResp[S_NOT_FOUND] = "Not Found";
    _stResp[S_METHOD_NOT_ALLOWED] = "Method Not Allowed";
    _stResp[S_LENGTH_REQUIRED] = "Length Required";
    _stResp[S_PAY_LOAD_TOO_LARGE] = "Payload Too Large";
    _stResp[S_URI_TOO_LONG] = "URI Too Long";
    _stResp[S_UNSUPPORTED_MEDIA_TYPE] = "Unsupported Media Type";
    _stResp[S_INTERNAL_SERVER_ERROR] = "Internal Server Error";
    _stResp[S_NOT_IMPLEMENTED] = "Not Implemented";
    _stResp[S_BAD_GATEWAY] = "Bad Gateway";
    _stResp[S_GATEWAY_TIMEOUT] = "Gateway Timeout";
    _stResp[S_HTTP_VERSION_NOT_SUPPORTED] = "HTTP Version Not Supported";
}

void Response::_startResponse()
{
	std::string _port = std::to_string(_httpServ.getPort());

	if (_request._getHeaderContent("port").compare(_port) == 0)
	{
		_makeStatus();
		_applyMethod();
		std::string _data;

		_data = _request._getHeaderContent("protocol");
		if (_data.compare("HTTP/1.1") == 0)
			_ResponseContent += _request._getHeaderContent("protocol");
		else
		{
			_status = S_HTTP_VERSION_NOT_SUPPORTED;
			_ResponseContent += "HTTP/1.1";
		}
		_ResponseContent += " ";
		_ResponseContent += std::to_string(_status);
		_ResponseContent += " ";
		_ResponseContent += _stResp[_status];
		_ResponseContent += "\r\n";
		if (_location.getIsRedirect())
		{
			_ResponseContent += "Location: ";
			_ResponseContent += _location.getRedirectUrl();
			_ResponseContent += "\r\n\r\n";
		}
		else
		{
			_ResponseContent += "Server: webserv/0.0\r\n";
			_ResponseContent += "Content-Type: text/html; charset=UTF-8\r\n";
			// if (_request._getHeaderContent("Content-Type").length())
				// _ResponseContent += _request._getHeaderContent("Content-Type");
			// else
			// _ResponseContent += "Content-Type: text/html\r\n";
			// _ResponseContent += "\r\n";
			_ResponseContent += "Content-Length: ";
			_ResponseContent += std::to_string(_body.length());
			_ResponseContent += "\r\n";
			_ResponseContent += "Connection: ";
			if (_request._getHeaderContent("Connection").length())
				_ResponseContent += _request._getHeaderContent("Connection");
			else if (_status != S_OK)
				_ResponseContent += "close\r\n";
			else
				_ResponseContent += "keep-alive\r\n";
			if (_request._getHeaderContent("method").compare("GET") == 0)
				_ResponseContent += "\r\n";
			_ResponseContent += "\r\n";
			_ResponseContent += _body;
		}
	}
}

std::string Response::_getResContent() const
{
    return (_ResponseContent);
}