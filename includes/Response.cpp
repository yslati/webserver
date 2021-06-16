#include "Response.hpp"
#include <regex>

Response::Response()
{
    _body = "";
    _ResponseContent = "";
    _status = 0;
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
	std::vector<HttpServer>::iterator it = _request._getIterator();

    if (!getcwd(buff, sizeof(buff)))
        std::cerr << "getcwd failed" << std::endl;
    else
	{
        dir = std::string(buff);
		// if there is a location path
		// dir.append(location.getRoot());
		// else
		// dir.append(defaultLocation);
	}
    return (dir);
}

std::string Response::_getFilePath(std::string& uri)
{
    std::string path = ""; 
    path = _getDir();
	if (uri.front() != "/")
		uri = "/" + uri;
    path.append(uri);
    return (path);
}

std::string Response::_getFileNameFromUri(std::string uri)
{
	// Parse filename from uri
	// then check if filename match any of locations spicified in the config file
	// if true break and return the filename
    return "/index.html";
}

void Response::_readFile(const std::string& file)
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
	if (_isDir(path))
	{
		// check if loaction->autoIndex == true
			// _applyAutoIndexin();
		// else
			// _status = ST_NOT_FOUND;
	}
	else
		_readFile(path);
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
    // for (size_t i = 0; i < 2; i++)
    // {
    //     Request::ArgContent arg = _request._getArg(i);
    //     std::string filename = _getFileNameFromDisp(arg._Cdisp);
    //     // std::string dir = _getDir().append("/").append(filename);
    //     // std::fstream file(dir);
    //     // file << arg._data;
    //     // file.close();
    // }
    for (size_t i = 0; i < _request._getVecCont().size(); i++)
    {
        Request::ArgContent arg = _request._getArg(i);
        _filename = _getFileNameFromDisp(arg._Cdisp);
        if (_filename.length())
        {
			_dir = _getDir().append("/").append(_filename);
			std::ofstream _file(_dir);
			// std::cout << "_data = " << arg._data; 
			// _file << arg._data;
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

void Response::_handleError()
{
	_body += "<html>\r\n";
	_body += "<head>\r\n";
	_body += "	<title>405 Not Allowed</title>\r\n";
	_body += "</head>\r\n";
	_body += "<body>\r\n";
	_body += "	<center>\r\n";
	_body += "		<h1>405 Not Allowed</h1>\r\n";
	_body += "	</center>\r\n";
	_body += "	<hr>\r\n";
	_body += "	<center>webserv/0.0</center>\r\n";
	_body += "</body>\r\n";
	_body += "</html>\r\n";
	_status = S_METHOD_NOT_ALLOWED;
}

void Response::_applyDeleteMethod()
{
    std::string path = _getFilePath(_request._getHeaderContent("uri"));
    if (_checkPermission(path, W_OK))
        _handleError();
    else
    {
		remove(path.c_str());
        _body = "<html>\r\n";
        _body += "	<body>\r\n";
        _body += "		<h1>File deleted.</h1>\r\n";
        _body += "	</body>\r\n";
        _body += "</html>\r\n";
		_status = S_OK;
    }
	/*if (_isDir(path))
	{
		// _status = ST_NOT_ALLOWED
		// throw Exeption
	}
	else
		_deleteFile(path);
	*/
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

std::string Response::_getHrefLink(std::string& dirname)
{
	std::stringstream ss;

	ss << "<p><a href=\"" + dirname + "\">" + dirname + "</p></a>\n\t\t";
	return ss.str();
}

int Response::_isDir(std::string& dirname)
{
	std::string _dir = _getDir();
	if (dirname.front() != '/')
		_dir += "/";
	_dir += dirname;
	if (!(opendir(_dir.c_str())))
		return 0;
	return 1;
}

void	Response::_applyAutoIndexing()
{
	DIR *dir;
	struct dirent *pDirent;
	std::string _line = "";
	std::string _name = "";
	std::string _uri = "/";
	std::string _data = "";
	std::string _index = "<h1>Index of: " + _uri + "</h1>\n\t\t<hr/>";

	_body.append(_generateHtmlTemplate());
	_body.replace(_body.find("$2"), 2, _index);
	if ((dir = opendir(_request._getHeaderContent("uri").c_str())))
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
}

void	Response::_applyMethod()
{
    std::vector<HttpServer>::iterator it = _request._getIterator();
    std::vector<Location> locations = it->getLocations();
    std::vector<Location>::iterator lit = locations.begin();
    for (; lit != locations.end(); lit++)
    {
        // if the uri match one of the paths in location
        // setLocation(*lit);
    }
    if (_request._getHeaderContent("method").compare("GET") == 0)
        _applyGetMethod();
    else if (_request._getHeaderContent("method").compare("POST") == 0)
        _applyPostMethod();
    else if (_request._getHeaderContent("method").compare("DELETE") == 0)
        _applyDeleteMethod();
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
    _applyMethod();
	_makeStatus();
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