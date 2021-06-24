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
        fds.push_back(c_it->getPfd());
        c_it++;
    }
}

void Server::acceptIncomingConnection(std::vector<struct pollfd>& fds, std::vector<Client>& clients) {
    for(int i = 0; i < _http_servers.size(); i++) {
        if (fds[i].revents & POLLIN) {
            std::cout << "Got a new connection" << std::endl;
            try
            {
                Client c(fds[i].fd);
                clients.push_back(c);
                // std::cout << _clients.size() << std::endl;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    }
}

void   Server::handle_read(std::vector<struct pollfd>& fds) {
  
}

void Server::poll_handle(std::vector<struct pollfd>& fds) {
    int n = poll(&(*fds.begin()), fds.size(), 3000);
    std::vector<Client> new_clients;
    if (n == 0) {
        std::cout << "Timeout" << std::endl;
    } else if (n > 0) {
        for (int i = 0; i < fds.size(); i++) {
            if (i < _http_servers.size()) {
                if (fds[i].revents & POLLIN) {
                    try
                    {
                        Client c(fds[i].fd);
                        new_clients.push_back(c);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                }
            }
        }
        // acceptIncomingConnection(fds, new_clients);
        // handle_read(fds);
        // std::vector<Client>::iterator c_it;
        // c_it = _clients.begin();
        // while (c_it != _clients.end())
        // {
        //     std::cout << "rd" << std::endl;
        //     if (c_it->getPfd().revents & POLLOUT) {
        //         if (c_it->readConnection()) {
        //             std::cout << "END" << std::endl;
        //             c_it->setReady(true);
        //         }
        //     }
        //     fds.push_back(c_it->getPfd());
        //     c_it++;
        // }
        _clients.assign(new_clients.begin(), new_clients.end());
    } else if (n == -1) {
        exit(EXIT_FAILURE);
    }
}

void Server::poll_loop() {
    while (true) {
        std::vector<struct pollfd> fds;
        int i = 0;
        std::cout << "FDS: ";
        while (i < _clients.size()) {
            std::cout << _clients[i].getConnection() << " ";
        }
        std::cout << std::endl;
        init_poll(fds);
        poll_handle(fds);
    }
}

void Server::acceptConnections() {
    std::cout << "start servers" << std::endl;
    start_servers();
    std::vector<HttpServer>::iterator h_it;
    while (true)
    {
        std::vector<struct pollfd> fds;
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
            fds.push_back(c_it->getPfd());
            c_it++;
        }
	std::vector<Client>::iterator it_c;
	it_c = _clients.begin();
	std::cout << "Fds: ";
	while (it_c != _clients.end())
	{
		std::cout << it_c->getConnection() << " ";
		it_c++;
	}
	std::cout << std::endl;
    int n = poll(&(*fds.begin()), fds.size(), -1);
    std::vector<Client> new_clients;
    std::set<int> toRemove;
	if (n == 0) {
            std::cout << "Timeout" << std::endl;
    } else if (n > 0) {
        for (int i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (i < _http_servers.size()) {
                    try
                    {
                        Client c(fds[i].fd);
                        std::cout << "new" << std::endl;
                        new_clients.push_back(c);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                } else {
                    // read connection
                    try
                    {
                        if (!_clients[i - _http_servers.size()].readConnection()) {
                            _clients[i - _http_servers.size()].setReady(true);
                        }
                    }
                    catch(const std::exception& e)
                    {
                            toRemove.insert(i - _http_servers.size());
                            std::cerr << e.what() << '\n';
                    }
                }
            } else if (fds[i].revents & POLLOUT) {
                try
                {
                    _clients[i - _http_servers.size()].writeConnection();
                    // if (_clients[i - _http_servers.size()].close)
                    //     toRemove.insert(i - _http_servers.size());
                }
                catch(const std::exception& e)
                {
                    toRemove.insert(i - _http_servers.size());
                    std::cerr << e.what() << '\n';
                }
                // _clients[i - _http_servers.size()].setReady(false);
            } else if (fds[i].revents & POLLHUP) {
                toRemove.insert(i - _http_servers.size());
            }
        }
    }
	std::set<int>::iterator it_set = toRemove.begin();
    it_set = toRemove.begin();
    std::vector<Client> tmp;
	for (size_t i = 0; i < _clients.size(); i++)
    {
        if (toRemove.find(i) == toRemove.end()) {
            tmp.push_back(_clients[i]);
        } else {
            close(_clients[i].getConnection());
        }
    }
    _clients.assign(tmp.begin(), tmp.end());
    _clients.insert(_clients.end(), new_clients.begin(), new_clients.end());
    }
}
