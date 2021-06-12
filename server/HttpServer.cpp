#include "HttpServer.hpp"

HttpServer::HttpServer() {
    _port = -1;
    _maxBodySize = -1;
}

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
        _host = rhs._host;
        // methods
        for(int i = 0; i < rhs._allowed_methods.size(); i++) {
            _allowed_methods.push_back(rhs._allowed_methods[i]);
        }

         for(int i = 0; i < rhs._locations.size(); i++) {
            _locations.push_back(rhs._locations[i]);
        }

    }
    return *this;
}


// ---------------------------CHECKERS--------------------------
void	HttpServer::checkVal() {
    std::cout << "server_name: \t" << _server_name << "\tPort: " << _port << std::endl;
    // std::cout << "allowed_methods: ";
    // for (int i = 0; i < _allowed_methods.size(); i++)
        // std::cout << _allowed_methods[i] << " ";
    std::cout << "\t\thost: " << _host << std::endl;
    std::cout << "\t\troot: " << _root << std::endl;
    std::cout << "============================" << std::endl;
}

// ----------------------------GETTERS--------------------------
std::string const& HttpServer::getServerName() const {
    return _server_name;
}

std::string const& HttpServer::getHost() const {
    return _host;
}

std::string const& HttpServer::getRoot() const {
    return _root;
}

std::vector<std::string> const& HttpServer::getAllowedMethods() const {
    return _allowed_methods;
}

int const& HttpServer::getPort() const {
    return _port;
}

int const& HttpServer::getMaxBodySize() const {
    return _maxBodySize;
}
// -----------------------------SETTERS-------------------------
void	HttpServer::setServerName(std::string const& x) {
    _server_name = x;
}

void	HttpServer::setHost(std::string const& x) {
    _host = x;
}

void	HttpServer::setRoot(std::string const& x) {
    _root = x;
}

void	HttpServer::setAllowedMethods(std::vector<std::string> x) {
    for (int i = 0; i < x.size(); i++) {
        if (x[i] != "GET" && x[i] != "POST" && x[i] != "DELETE")
            throw "allowed method not acceptable";
        _allowed_methods.push_back(x[i]);
    }
}

void	HttpServer::setPort(int const& x) {
    _port = x;
}

void	HttpServer::setMaxBodySize(int const& x) {
    _maxBodySize = x;
}

void	HttpServer::addLocation(Location const& loc) {
    for (int i = 0; i < _locations.size(); i++) {
		if (_locations[i].getUri() == loc.getUri())
			throw "Location duplicated";
    }
    _locations.push_back(loc);
}

// ----------------------------OVERLOADS-----------------------

void	HttpServer::addErrorPage(int statusCode, std::string path) {
    for (std::map<int, std::string>::iterator it = _errors.begin(); it != _errors.end(); it++) {
        if (it->first == statusCode)
            throw "Status Code duplicated";
    }
    _errors.insert(std::pair<int, std::string>(statusCode, path));
    // ErrorPage ep(statusCode, path);
    // _error_pages.push_back(ep);
}
