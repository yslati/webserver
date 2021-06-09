#include "HttpServer.hpp"

HttpServer::HttpServer() {}

HttpServer::HttpServer(int port, std::string server_name) {
    _port = port;
    _server_name = server_name;
}

HttpServer::HttpServer(HttpServer const& src) {
    *this = src;
}

HttpServer& HttpServer::operator=(HttpServer const& rhs) {
    if (this != &rhs) {
        _port = rhs._port;
        _server_name = rhs._server_name;
    }
    return *this;
}


// ---------------------------CHECKERS--------------------------
void	HttpServer::checkVal() const {
    std::cout << "server_name: \t" << _server_name << "\tPort: " << _port << std::endl;
    std::cout << "allowed_methods: ";
    // for (int i = 0; i < _allowed_methods.size(); i++)
        std::cout << _allowed_methods[0] << " ";
    std::cout << "\t\thost: " << _host << std::endl;
    std::cout << "============================" << std::endl;
}

// ----------------------------GETTERS--------------------------
std::string const& HttpServer::getServerName() const {
    return _server_name;
}

std::string const& HttpServer::getHost() const {
    return _host;
}

std::vector<std::string> const& HttpServer::getAllowedMethods() const {
    return _allowed_methods;
}

int const& HttpServer::getPort() const {
    return _port;
}
// -----------------------------SETTERS-------------------------
void	HttpServer::setServerName(std::string const& x) {
    _server_name = x;
}

void	HttpServer::setHost(std::string const& x) {
    _host = x;
}

void	HttpServer::setAllowedMethods(std::vector<std::string> x) {
    for (int i = 0; i < x.size(); i++)
        _allowed_methods.push_back(x[i]);
}

void	HttpServer::setPort(int const& x) {
    _port = x;
}
// ----------------------------OVERLOADS-----------------------

void	HttpServer::addErrorPage(int statusCode, std::string path) {
    _errors.insert(std::pair<int, std::string>(statusCode, path));
    // ErrorPage ep(statusCode, path);
    // _error_pages.push_back(ep);
}
