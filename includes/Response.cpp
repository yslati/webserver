#include "Response.hpp"

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

std::string Response::_getDir(void)
{
    char buff[1024];
    std::string dir = "";

    if (!getcwd(buff, sizeof(buff)))
        std::cerr << "getcwd failed" << std::endl;
    else
        dir = std::string(buff);
    return (dir);
}

std::string Response::_getFilePath(const std::string& uri)
{
    std::string path = "";
    path = _getDir();
    path.append("/server/_dhtml");
	if (uri.compare("/") == 0)
    	path.append("/index.html");
	else
    	path.append(uri);
    return (path);
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
    std::string path = _getFilePath(_request._getHeaderContent("uri"));
	if (_checkPermission(path, R_OK))
		_handleError();
	else
    	_readFile(path);
}

std::string Response::_getFileNameFromDisp(std::string disp)
{
	std::string path = disp.substr(disp.find("filename=") + 1,
	disp.find_last_of("\"") - 1);
	std::cout << "hh = " << path << std::endl;
	return "hey";
}

void Response::_applyPostMethod()
{
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
	// for (std::vector<Request::ArgContent>::iterator it = _request._getVecCont().begin();
	// it != _request._getVecCont().end(); it++)
	// {
	// 	Request::ArgContent arg = *it;
	// 	std::cout << arg._Ctype << std::endl;
	// 	std::cout << arg._Cdisp << std::endl;
	// 	std::cout << arg._data << std::endl;
	// }
    _body += "File uploaded";
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
}

void Response::_applyMethod()
{
    if (_request._getHeaderContent("method").compare("GET") == 0)
        _applyGetMethod();
    else if (_request._getHeaderContent("method").compare("POST") == 0)
        _applyPostMethod();
    else if (_request._getHeaderContent("method").compare("DELETE") == 0)
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
    // _body
    // _ResponseContent.append(_request._getProtocol());
    // _ResponseContent += _request._getProtocol();
    // // _ResponseContent.append(" ");
    // _ResponseContent += " ";
    // // _ResponseContent.append(std::to_string(_status));
    // _ResponseContent += std::to_string(_status);
    // _ResponseContent += " ";
    // // _ResponseContent.append(_stResp[_status]);
    // _makeStatus();
    // _ResponseContent +=  _stResp[_status];
    // std::cout << "p = " << _ResponseContent << "\n";
    // std::cout << "OK\n";
    // HTTP/1.1 
    // _ResponseContent = "HTTP/1.1 200 OK\r\nContent-Length: 10\r\n\r\nhello worl";
    // std::cout << _ResponseContent << std::endl;
}

std::string Response::_getResContent() const
{
    return (_ResponseContent);
}