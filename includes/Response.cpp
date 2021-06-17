#include "Response.hpp"
#include <regex>

Response::Response(Location& location): _location(location)
{
    _body = "";
    _ResponseContent = "";
    _status = 0;
	// _location = new Location();
}

Response::~Response()
{
    _body = "";
    _ResponseContent = "";
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
		if (_location.getRoot().length())
		{
			if (_location.getRoot().front() != '/')
				dir += "/";
			dir.append(_location.getRoot());
		}
		else
		{
			std::cout << "rootDefault = " <<
			_request._getIterator()->getRoot() << "\n";
			if (_request._getIterator()->getRoot().length() &&
			_request._getIterator()->getRoot().front() != '/')
				dir += "/";
			dir.append(_request._getIterator()->getRoot());
		}
	}
    return (dir);
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
    return (filename.append(_location.getIndex()));
}

void Response::_readFile(std::string file)
{
    std::string _line;
    std::string body = "";
    std::ifstream input_file(file);

    if (input_file.fail())
    {
        _status = S_NOT_FOUND;
        std::string path = _getFilePath("/ErrorPage.html");
        std::ifstream error_file(path);

        if (error_file.fail())
            std::cerr << "Error occured = " << path << "\n";
        while (getline(error_file, _line))
            body.append(_line).append("\n");
        _body = body;
        return ;
    }
    while (getline(input_file, _line))
        body.append(_line).append("\n");
    _body = body;
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
	std::cout << "path = " << path << std::endl;
	if (_isDir(path))
	{
		// std::cout  << "path = " << path << "\n";
		// std::cout  << "bool = " << _location.getAutoIndex() << "\n";
		if (_location.getAutoIndex())
			_applyAutoIndexing(path);
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
		// path = disp.substr(disp.find("filename=\"") + 10,
		// disp.length() - disp.substr(0, disp.find("filename") + 11).length());
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
			_dir = _getDir().append("/").append(_filename);
			std::ofstream _file(_dir);
			std::istringstream ss(arg._data);

			while (getline(ss, _line)) {
				_file << _line.substr(0, _line.find("\r\n")).append("\n");
			}
			_file.close();
		}
	}
	_body = "<html>\r\n";
	_body += "	<body>\r\n";
	_body += "		<h1>File uploaded.</h1>\r\n";
	_body += "	</body>\r\n";
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
	_body = "<html>\n";
	_body += "<head>\n";
	_body += "	<title>$1</title>\n";
	_body += "</head>\n";
	_body += "<body>\n";
	_body += "	<center>\n";
	_body += "		<h1>";
	_body += "$1";
	_body += "</h1>\n";
	_body += "	</center>\n";
	_body += "	<hr>\n";
	_body += "	<center>webserv/0.0</center>\n";
	_body += "</body>\n";
	_body += "</html>\n";
	_body.replace(_body.find("$1"), 2, _stResp[_status]);
	_body.replace(_body.find("$1"), 2, _stResp[_status]);
}

void Response::_deleteFile(std::string _file)
{
	// if (_checkPermission(_file, W_OK))
	// 	throw Response::PermissionDiend();
	if (!(std::remove(_file.c_str())))
	{
		_status = S_METHOD_NOT_ALLOWED;
		std::cout << "file not found\n";
	}
	else
	{
		_body = "<html>\r\n";
		_body += "	<body>\r\n";
		_body += "		<h1>File deleted.</h1>\r\n";
		_body += "	</body>\r\n";
		_body += "</html>\r\n";
		_status = S_OK;
	}
}

void Response::_applyDeleteMethod()
{
    std::string path = _getFilePath(_request._getHeaderContent("uri"));

    // if (_checkPermission(path, W_OK))
	// {
	// 	_status = S_METHOD_NOT_ALLOWED;
    //     _generateErrorPage();
	// }
    // else
    // {
	// 	remove(path.c_str());
    //     _body = "<html>\r\n";
    //     _body += "	<body>\r\n";
    //     _body += "		<h1>File deleted.</h1>\r\n";
    //     _body += "	</body>\r\n";
    //     _body += "</html>\r\n";
	// 	_status = S_OK;
    // }
	if (_isDir(path))
	{
		_status = S_FORBIDDEN;
		// if (file.error)
		// 	_body = contentHtml;
		// else
		_generateErrorPage();
		std::cout << _body << std::endl;
		// if ()
	}
	else
		_deleteFile(path);
	
}


std::string Response::_generateHtmlTemplate()
{
	return "<!DOCTYPE html>\n\
<html lang=\"en\">\n\
<head>\n\
    <meta charset=\"UTF-8\">\n\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
    <title>WebServer</title>\n\
</head>\n\
<body>\n\
		$2\n\
        $1\n\
</body>\n\
</html>\n";
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
	if (_location.getIsRedirect())
		_handleRedirect();
	else if (_request._getHeaderContent("method").compare("GET") == 0)
		_applyGetMethod();
	// else if (_request._getHeaderContent("method").compare("POST") == 0
	// && _checkAllowedMethod("POST"))
	// 	_applyPostMethod();
	else if (_request._getHeaderContent("method").compare("DELETE") == 0
	&& _checkAllowedMethod("DELETE"))
	{
		std::cout << "here" << "\n";
		_applyDeleteMethod();
	}
	else
		_status = S_NOT_IMPLEMENTED;
	if (_status != S_OK)
		_status = S_OK;
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
	_ResponseContent += "Server: ";
	_ResponseContent += " webserv/0.0\r\n";
	if (_request._getHeaderContent("Content-Type").length())
		_ResponseContent += _request._getHeaderContent("Content-Type");
	else
		_ResponseContent += "Content-Type: text/html";
	_ResponseContent += "\r\n";
    _ResponseContent += "Content-Length: ";
    _ResponseContent += std::to_string(_body.length());
	_ResponseContent += "\r\n";
	_ResponseContent += "Connection: ";
	if (_request._getHeaderContent("Connection").length())
		_ResponseContent += _request._getHeaderContent("Connection");
	else if (_status != S_OK)
		_ResponseContent += "close";
    else
        _ResponseContent += "keep-alive";
    _ResponseContent += "\r\n\r\n";
    _ResponseContent += _body;
}

std::string Response::_getResContent() const
{
    return (_ResponseContent);
}