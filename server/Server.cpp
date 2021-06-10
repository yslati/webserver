#include "Server.hpp"

Server& Server::getInstance() {
    if (!_instance) {
        _instance = new Server();
    }
    return *_instance;
}

void Server::addHttpServer(HttpServer const& server) {
    _http_servers.push_back(server);
    
    HttpServer &sr = _http_servers.back();
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

void Server::acceptConnections() {
    std::vector<ServerSocket>::iterator it = _sockets.begin();
    // FD_ZERO(&master_set);
    // FD_ZERO(&response_set);
    // addServerSocketsToSet();

        // worker_set = master_set;
        // response_set = master_set;
        // std::cout << "Select Waiting" << std::endl;
        // int select_ret = select(FD_SETSIZE, &worker_set, &response_set, NULL, NULL);
        // if (select_ret == 0)
        // {
        //     std::cout << "select 0" << std::endl;
        //     continue;
        // }
        // // std::cout << "ret = " << select_ret << std::endl;
        // for (int i = 0; i < FD_SETSIZE; i++) {
        //     if (FD_ISSET(i, &response_set)) {
        //         std::string _str = "HTTP/1.1 200 OK\r\nContent-Type:text/html\r\nContent-Length: 10\r\nHello world";
        //         send(i, _str.c_str(), _str.size(), 0);
        //     }
        //     // if ready to read or accept a socket
        //     if (FD_ISSET(i, &worker_set)) {
        //         if ((it =isServerSocket(i)) != _sockets.end()) {
        //             int conn = it->acceptConnection();
        //             FD_SET(conn, &master_set);
        //         } else {
        //             RequestReader reader;
        //             reader.readConnection(i);
        //         }
        //     }

        // }
        // fd_set read_set;
        // fd_set write_set;

        // FD_ZERO(&read_set);
        // FD_ZERO(&write_set);

        // std::vector<ServerSocket>::iterator it;
        // for (it = _sockets.begin(); it != _sockets.end(); it++) {
        //     if (it->getFd() != -1) {
        //         FD_SET(it->getFd(), &read_set);
        //     }
        // }

        while (true)
        {
            // struct timeval t = { 5, 0 };
            // std::cout << "Select" << std::endl;
            // int select_ret = select(FD_SETSIZE, &read_set, &write_set, NULL, NULL);
            // if (select_ret == 0)
            //     continue;
            // for (size_t i = 0; i < FD_SETSIZE; i++)
            // {
            //     if (FD_ISSET(i, &read_set)) {
            //         if (FD_ISSET(i, &worker_set)) {
            //             if ((it =isServerSocket(i)) != _sockets.end()) {
            //                 int conn = it->acceptConnection();
            //                 std::cout << "Got a connection" << std::endl;
            //                 FD_SET(conn, &read_set);
            //             } else {
            //                 std::cout << "Conn i should read from" << std::endl;
            //             }
            //         }
            //     }
            // }

            std::cout << it->getPort() << std::endl;
            int conn = it->acceptConnection();
            char buffer[1024];
            int ret;
            std::string reqContent;

            while ((ret = read(conn, buffer, sizeof(buffer) - 1)) == 1023) {
                std::cout << ret << std::endl;
                buffer[ret] = '\0';
                std::string tmp = "";
                tmp.assign(buffer);
                reqContent += tmp;
            }
            
            std::string tmp = "";
            tmp.assign(buffer);
            reqContent += tmp;

            std::cout << reqContent << std::endl;
            std::cout << "End" << std::endl;
            close(conn);
        }
}