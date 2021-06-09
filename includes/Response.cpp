#include "Response.hpp"

Response::Response()
{
    _body = "";
    _ResponseContent = "";
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

const std::string& Response::_getFilePath(const std::string& uri)
{
    std::string path = "";
    path = _getDir();
    path.append("/public/");
    path.append(uri);
    return (path);
}

void Response::_readFile(const std::string& file)
{
    std::string _line;
    std::string body = "";
    std::ifstream input_file(file);

    while (getline(input_file, _line))
        body.append(_line).append("\n");
    _body = body;
}

void Response::_applyGetMethod()
{
    std::string path = _getFilePath(_request._getUri());
    _readFile(path);
}

void Response::_applyMethod()
{
    if (_request._getMethod().compare("GET") == 0)
        _applyGetMethod();
    else if (_request._getMethod().compare("POST") == 0)
        _applyPostMethod();
    else if (_request._getMethod().compare("DELETE") == 0)
        _applyDeleteMethod();
    else
        _status = S_NOT_IMPLEMENTED;
    _status = S_OK;
}

void Response::_makeStatus()
{
    std::pair<int, std::string> p;
    p.first = S_OK;
    p.second = "OK";

    _stResp.insert(p);
}

void Response::_startResponse()
{
    _applyMethod();
    _ResponseContent.append(_request._getProtocol());
    _ResponseContent.append(" ");
    _ResponseContent.append(std::to_string(_status));
    _ResponseContent.append(_stResp[_status]);
}