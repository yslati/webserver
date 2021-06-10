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
    // std::cerr << "path = " << path << "\n";
    _readFile(path);
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
    if (!_status)
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

    _ResponseContent += _request._getHeaderContent("protocol");
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