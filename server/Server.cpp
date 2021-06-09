#include "Server.hpp"

Server& Server::getInstance() {
    if (!_instance) {
        _instance = new Server();
    }
    return *_instance;
}

void Server::addHttpServer(HttpServer const& server) {
    _http_servers.push_back(server);
    ServerSocket ss(server.getPort());
    addSocket(ss);
}

std::vector<ServerSocket> const& Server::getSockets() const {
    return _sockets;
}

std::vector<HttpServer> const& Server::getHttpServers() const {
    return _http_servers;
}

// polymorph socketCreate
void Server::socketsCreate() {
    std::vector<ServerSocket>::iterator it;
    for (it = _sockets.begin(); it != _sockets.end(); it++) {
        it->socketCreate();
    }
}

void Server::socketsBind() {
    std::vector<ServerSocket>::iterator it;
    for (it = _sockets.begin(); it != _sockets.end(); it++) {
        it->socketBind();
    }
}

void Server::socketsListen() {
    std::vector<ServerSocket>::iterator it;
    for (it = _sockets.begin(); it != _sockets.end(); it++) {
        it->socketListen();
    }
}

Server::Server() {}

void Server::addSocket(ServerSocket const& ss) {
    std::vector<ServerSocket>::iterator it;
    it = std::find(_sockets.begin(), _sockets.end(), ss);
    if (it == _sockets.end()) {
        _sockets.push_back(ss);
    }
}

Server* Server::_instance = nullptr;

void Server::addServerSocketsToSet() {
    std::vector<ServerSocket>::iterator it;
    for (it = _sockets.begin(); it != _sockets.end(); it++) {
        if (it->getFd() != -1) {
            FD_SET(it->getFd(), &master_set);
        }
    }
}

std::vector<ServerSocket>::iterator Server::isServerSocket(int fd) {
    std::vector<ServerSocket>::iterator it;
    for (it = _sockets.begin(); it != _sockets.end(); it++) {
        if (it->getFd() == fd) {
            return it;
        }
    }
    return it;
}

void Server::acceptConnections() {
    std::vector<ServerSocket>::iterator it;
    FD_ZERO(&master_set);
    addServerSocketsToSet();
    while (true) {
        worker_set = master_set;
        response_set = master_set;
        std::cout << "Select Waiting" << std::endl;
        if (select(FD_SETSIZE, &worker_set, NULL, NULL, NULL) < 0) {
            std::cerr << "Select failed" << std::endl;
            continue;
        }
        for (int i = 0; i < FD_SETSIZE; i++) {
            // if ready to read or accept a socket
            if (FD_ISSET(i, &worker_set)) {
                if ((it =isServerSocket(i)) != _sockets.end()) {
                    int conn = it->acceptConnection();
                    FD_SET(conn, &master_set);
                } else {
                    std::cout << "Connection should we read from" << std::endl;
                    char buffer[1025];
                    std::string _s;
                    int ret;
                    while ((ret = recv(i, buffer, 1024, 0)) > 0) {
                        buffer[ret] = '\0';
                        std::cout << buffer << std::endl;
                    }
                    FD_CLR(i, &master_set);
                }
            }
        }
    }
}