#include "Server.hpp"
#include "poll.h"
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

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
    bool running = true;
    std::vector<int> _sockets_fd;
    int serverSocketCount = 0;

    // add server sockets
    std::vector<ServerSocket>::iterator it;
    for (it = _sockets.begin(); it != _sockets.end(); it++) {
        _sockets_fd.push_back(it->getFd());
        serverSocketCount++;
    }

    while (running)
    {
        struct pollfd fds[_sockets_fd.size()];
        for (int i = 0; i < _sockets_fd.size(); i++) {
            fds[i].fd = _sockets_fd[i];
            fds[i].events = POLLIN;
        }
        log "Poll" line;
        int n = poll(fds, _sockets_fd.size(), 2000);
        if (n == 0)
            continue;
        for (int i = 0; i < _sockets_fd.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (i < serverSocketCount) {
                    int conn = _sockets[i].acceptConnection();
                    _sockets_fd.push_back(conn);
                    fcntl(conn, F_SETFL, O_NONBLOCK);
                    std::cout << "Got a new connection" << std::endl;
                } else {
                    std::cout << "Connection i should read from" << std::endl;
                    int ret;
                    char buffer[1024];
                    std::string reqContent = "";
                    while ((ret = recv(fds[i].fd, buffer, 1023, 0)) == 1024) {
                        std::string tmp;
                        buffer[ret] = '\0';
                        tmp.assign(buffer);
                        reqContent += tmp;
                    }
                    if (ret != -1) {
                        std::string tmp;
                        buffer[ret] = '\0';
                        tmp.assign(buffer);
                        reqContent += tmp;
                        std::cout << reqContent << std::endl;
                    }
                }
            }
        }
    }
}