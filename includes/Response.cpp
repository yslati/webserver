#include "Response.hpp"
#include <regex>

Response::Response(Location& location, HttpServer& httpServ): _location(location), _httpServ(httpServ)
{
	// std::cout << "wa si = " << _location.getRoot() << std::endl;
    _body = "";
    _ResponseContent = "";
	_st = "";
	_Ctype = "";
	_loc = "";
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

std::string		Response::_getKey(std::string ctype)
{
	std::string path = "";
    std::regex re("name=\"");
    std::smatch match;
    if (ctype.length())
    {
        std::regex_search(ctype, match, re);
        if (!match.empty())
        {
            path = match.suffix();
            path = path.substr(0, path.find("\""));
        }
    }
	return path;
}

int Response::_runCgi()
{
	int fds1[2];
	int fds2[2];
	char **_env;
	char **args;
	int status;


	std::string tmp = "SCRIPT_FILENAME=" + _scriptFileName;
	std::string PATH = "PATH='/usr/bin/:/Users/yslati/.brew/bin/'";
	std::string query_string = "QUERY_STRING=" + _request._getHeaderContent("query_string");
	std::string method = "REQUEST_METHOD=" + _request._getHeaderContent("method");
	std::string st = "REDIRECT_STATUS=" + std::to_string(200);
	std::string cn = "CONTENT_LENGTH=" + _request._getHeaderContent("Content-Length");
	std::string ctype = "CONTENT_TYPE=" + _request._getHeaderContent("Content-Type");
	// std::string ctype = "CONTENT_TYPE=application/json";
	// std::string ctype = "CONTENT_TYPE=multipart/form-data; boundary=" + _request._getBoundary();
	
	std::string postdata = _request._getPostBody();

	std::cout << "postdata = " << postdata << "\n";
	std::cout << "len = " << _request._getHeaderContent("Content-Length") << "\n";
	std::cout << "type = " << _request._getHeaderContent("Content-Type") << "\n";
	std::cout << "method = " << _request._getHeaderContent("method") << "\n";
	std::cout << "query_string = " << _request._getHeaderContent("query_string") << "\n";
	std::cout << "_scriptFileName = " << _scriptFileName << "\n";

	// for (size_t i = 0; i < _request._getVecCont().size(); i++)
	// {
	// 	Request::ArgContent arg = _request._getArg(i);
	// 	if (arg._data.length() && arg._Cdisp.length())
	// 	{
	// 		arg._Cdisp.pop_back();
	// 		// arg._data.pop_back();
	// 		std::cout << "l = " << arg._data << std::endl;
	// 		body.append("\"" + _getKey(arg._Cdisp) + "=" + arg._data + "&");
	// 	}
	// }
	// body.pop_back();
	
	// std::cout << "bd = " << body << std::endl;

	_env = (char **)malloc(sizeof(char *) * 8);
	_env[0] = strdup(tmp.c_str());
	_env[1] = strdup(PATH.c_str());
	_env[2] = strdup(query_string.c_str());
	_env[3] = strdup(method.c_str());
	_env[4] = strdup(st.c_str());
	_env[5] = strdup(cn.c_str());
	_env[6] = strdup(ctype.c_str());
	_env[7] = NULL;

	if (_location.getPhpCGI())
	{
		args = (char**)malloc(sizeof(char *) * 2);
		args[0] = strdup(_location.getFastcgiPass().c_str());
		args[1] = NULL;
	}
	else if (_location.getNodeCGI())
	{
		args = (char**)malloc(sizeof(char *) * 3);
		args[0] = strdup(_location.getFastcgiPass().c_str());
		args[1] = strdup(_scriptFileName.c_str());
		args[2] = NULL;
	}
	else if (_location.getPyCGI())
	{
		args = (char**)malloc(sizeof(char *) * 3);
		args[0] = strdup(_location.getFastcgiPass().c_str());
		args[1] = strdup(_scriptFileName.c_str());
		args[2] = NULL;
	}

	pipe(fds1);
	pipe(fds2);
	pid_t pid = fork();
	if (!pid)
	{
		close(fds1[1]);
		close(fds2[0]);
		dup2(fds2[1], 1);
		dup2(fds1[0], 0);
		execve(args[0], args, _env);
		exit(0);
	}
	else
	{
		close(fds1[0]);
		close(fds2[1]);
		write(fds1[1], postdata.c_str(), postdata.length());
        close(fds1[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			std::cout << "Status: ";
			status = WEXITSTATUS(status);
		}
	}
	if (status == 1)
		return -1;
	return (fds2[0]);
}

// int main(int ac, char **av, char **env) {
// 	int fd = runCgi(env);

// 	int r;
// 	char c;

// 	while ((r = read(fd, &c, 1)) > 0)
// 		write(1, &c, 1);
// 	return 0;
// }

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
	if (_location.getFastcgiPass().length())
		return (1);
	return (0);
}

void	Response::_handleCGI()
{
	int fd = _runCgi();
	char buffer[1024];
	int r;
	std::string body = "";
	bool _isEmpty = false;
	std::string _line;

	if (fd < 0)
	{
		_status = 500;
		_generateErrorPage();
		return ;
	}
	while ((r = read(fd, buffer, sizeof(buffer))) > 0)
		buffer[r] = '\0';
	std::cout << buffer << std::endl;
	std::istringstream _read(buffer);

	while (getline(_read, _line))
	{
		if (_line.find("Content-type: ") != std::string::npos)
			_Ctype = _line.substr(_line.find("Content-Type: ") + 14);
		else if (_line.find("Status: ") != std::string::npos)
			_st = _line.substr(_line.find("Status: ") + 8);
		else if (_line.find("Location: ") != std::string::npos)
			_loc = _line.substr(_line.find("Location: " + 10));
		else
		{
			if (_isEmpty)
			{
				if (_line.length())
					body.append(_line).append("\r\n");
			}
			else
				_isEmpty = true;
		}
	}
	_body = body.append("\r\n");
	_status = S_OK;
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

std::string Response::_getScriptFileName() const
{
	return _scriptFileName;
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

void	Response::_handleError()
{
	if (_httpServ._getErrorPages(_status).length())
		_readErrorPageFile(_httpServ._getErrorPages(_status));
	else
		_generateErrorPage();
}

void Response::_readFile(std::string file)
{
	std::string _line;
	std::string body = "";
	std::ifstream input_file(file);

	if (input_file.fail())
	{
		_status = S_NOT_FOUND;
		_handleError();
		// if (_httpServ._getErrorPages(_status).length())
		// 	_readErrorPageFile(_httpServ._getErrorPages(_status));
		// else
		// 	_generateErrorPage();
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

	if (_location.getUri().compare(_request._getHeaderContent("uri")) == 0
	&& !_location.getAutoIndex() && _isDir(path))
	{
		if (path.back() != '/')
			path += "/";
		path.append(_location.getIndex());
	}
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
		{
			_status = S_OK;
			path.append(_location.getIndex());
			if (!_checkPermission(path, R_OK))
				_readFile(path);
			else
			{
				_status = S_FORBIDDEN;
				_handleError();
			}
			// _status = S_FORBIDDEN;
			// _handleError();
			// if (_httpServ._getErrorPages(_status).length())
			// 	_readErrorPageFile(_httpServ._getErrorPages(_status));
			// else
			// 	_generateErrorPage();
		}
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
	_body += "\t<body>\r\n";
	_body += "\t\t<h1>Content uploaded.</h1>\r\n";
	_body += "\t</body>\r\n";
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
	if (!_checkPermission(_file, W_OK) && std::remove(_file.c_str()) == 0)
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
		_status = S_FORBIDDEN;
		_handleError();
	}
}

void Response::_applyDeleteMethod()
{
    std::string path = _getFilePath(_request._getHeaderContent("uri"));

	std::cout << "path_del = " << path << std::endl;
	if (_isDir(path))
	{
		_status = S_FORBIDDEN;
		_handleError();
		// if (_httpServ._getErrorPages(_status).length())
		// 	_readErrorPageFile(_httpServ._getErrorPages(_status));
		// else
		// 	_generateErrorPage();
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

bool	Response::_matchEnd(std::string s1, std::string s2)
{
	int n1 = s1.length(), n2 = s2.length();
	if (n1 > n2 || !s1.length() || !s2.length())
		return false;
	for (int i = 0; i < n1; i++)
		if (s1[n1 - i - 1] != s2[n2 - i - 1])
			return false;
	return true;
}

std::string		Response::_getContentType()
{
	if (_Ctype.length())
	{
		if (_Ctype.find(";") != std::string::npos)
			_Ctype = _Ctype.substr(0, _Ctype.find(";"));
		return _Ctype;
	}
	// else if (_matchEnd(".html", uri))
	// 	return "text/html";
	return "text/html";
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
	_scriptFileName = _getFilePath(_getFileNameFromUri(_request._getHeaderContent("uri")));

	
	if (_scriptFileName.find("?") != std::string::npos)
		_scriptFileName = _scriptFileName.substr(0, _scriptFileName.find("?"));

	if (_request._getError())
	{
		_status = S_BAD_REQ;
		_handleError();
	}
	else if (_httpServ.getMaxBodySize() != -1
	&& _request._getContentLen() > _httpServ.getMaxBodySize())
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
	{
		_status = S_NOT_IMPLEMENTED;
		_handleError();
	}
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

bool Response::_toClose()
{
	if (_status != S_OK)
		return true;
	return false;
}

void Response::_startResponse()
{
	std::string _port = std::to_string(_httpServ.getPort());
	std::string _data;

	if (_request._getHeaderContent("port").compare(_port) == 0)
	{
		_makeStatus();
		_applyMethod();

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
			_ResponseContent += "Content-Type: ";
			// _ResponseContent += "text/html";
			_ResponseContent += _getContentType();
			_ResponseContent += "\r\n";
			_ResponseContent += "Content-Length: ";
			_ResponseContent += std::to_string(_body.length());
			_ResponseContent += "\r\n";
			_ResponseContent += "Connection: ";
			if (_request._getHeaderContent("Connection").length())
				_ResponseContent += _request._getHeaderContent("Connection");
			else if (_status != S_OK)
				_ResponseContent += "close\r\n";
			else if (_request._getHeaderContent("method").compare("POST") == 0)
				_ResponseContent += "keep-alive\r\n";
			_ResponseContent += "\r\n";
			if (_request._getHeaderContent("method").compare("GET") == 0 ||
			_request._getHeaderContent("method").compare("DELETE") == 0)
				_ResponseContent += "\r\n";
			_ResponseContent += "\r\n";
			_ResponseContent += _body;
		}
	}
	std::cout << _ResponseContent << std::endl;
}

std::string Response::_getResContent() const
{
    return (_ResponseContent);
}