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

Server::Server() {

}

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

void Server::start_servers() {
    std::set< std::vector<HttpServer>::iterator > toRemove;
    std::vector<HttpServer>::iterator it;
    it = _http_servers.begin();
    while (it != _http_servers.end())
    {
        try
        {
           it->start_listen();
        }
        catch(const std::exception& e)
        {
            toRemove.insert(it);
            std::cerr << e.what() << '\n';
        }
        it++;
    }
    std::set< std::vector<HttpServer>::iterator >::iterator its;
    its = toRemove.begin();
    while (its != toRemove.end()) {
        _http_servers.erase(*its);
        its++;
    }
}

void    Server::init_poll(std::vector<struct pollfd>& fds) {
    std::vector<HttpServer>::iterator h_it;

    // server fds
    h_it = _http_servers.begin();
    while (h_it != _http_servers.end()) {
        fds.push_back((struct pollfd){h_it->getFd(), POLLIN});
        h_it++;
    }
    // client fds
    std::vector<Client>::iterator c_it;
    c_it = _clients.begin();
    while (c_it != _clients.end())
    {
        fds.push_back((struct pollfd){c_it->getConnection(), POLLIN});
        c_it++;
    }
}

void Server::acceptIncomingConnection(std::vector<struct pollfd>& fds) {
    for(int i = 0; i < _http_servers.size(); i++) {
        if (fds[i].revents == POLLIN) {
            std::cout << "Got a new connection" << std::endl;
            try
            {
                Client c(fds[i].fd);
                _clients.push_back(c);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    }
}

void   Server::handle_read(std::vector<struct pollfd>& fds) {
    int i = _http_servers.size() - 1;
    while (i < fds.size()) {
        if (fds[i].revents & POLLIN) {
          try
          {
            if (!_clients[i].readConnection()) {
                std::cout << "Client ready" << std::endl;
            } else {
                std::cout << _clients[i].getContent();
            }
          }
          catch(const std::exception& e)
          {
              std::cerr << e.what() << '\n';
          }
          
        }
    }
}

void Server::poll_handle(std::vector<struct pollfd>& fds) {
    int n = poll(&(*fds.begin()), fds.size(), 3000);
    if (n == 0) {
        std::cout << "Timeout" << std::endl;
    } else if (n > 0) {
        acceptIncomingConnection(fds);
    }
}

void Server::poll_loop() {
    while (true) {
        std::vector<struct pollfd> fds;
        init_poll(fds);
        poll_handle(fds);
    }
}

void Server::acceptConnections() {
    std::cout << "start servers" << std::endl;
    start_servers();
    if (_http_servers.size() > 0) {
        std::cout << "start poll loop" << std::endl;
        poll_loop();
    }
}