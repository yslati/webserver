#ifndef SERVER_H
# define SERVER_H
# include <iostream>
# include <vector>
# include "HttpServer.h"
# include "ServerSocket.h"

class Server {
	public:
		static Server& getInstance() {
			if (!_instance) {
				_instance = new Server();
			}
			return *_instance;
		}

		void addHttpServer(HttpServer const& server) {
			_http_servers.push_back(server);
			ServerSocket ss(server.getPort());
			addSocket(ss);
		}

		std::vector<ServerSocket> const& getSockets() const {
			return _sockets;
		}

		std::vector<HttpServer> const& getHttpServers() const {
			return _http_servers;
		}

		// polymorph socketCreate
		void socketsCreate() {
			std::vector<ServerSocket>::iterator it;
			for (it = _sockets.begin(); it != _sockets.end(); it++) {
				it->socketCreate();
			}
		}
	private:
		Server() {}
		static Server* _instance;
		std::vector<HttpServer> _http_servers;
		std::vector<ServerSocket> _sockets;

		// add socket
		void addSocket(ServerSocket const& ss) {
			std::vector<ServerSocket>::iterator it;
			it = std::find(_sockets.begin(), _sockets.end(), ss);
			if (it == _sockets.end()) {
				_sockets.push_back(ss);
			}
		}
};

Server* Server::_instance = nullptr;

std::ostream& operator<<(std::ostream& o, Server const& rhs) {
	o << "Server:\n";
	o << "************ servers  ************" << std::endl;
	std::vector<HttpServer>::const_iterator its;
	for (its = rhs.getHttpServers().begin(); its != rhs.getHttpServers().end(); its++) {
		o << *its;
	}
	o << "************ end servers  ************" << std::endl;
	o << "************ sockets ************" << std::endl;
	std::vector<ServerSocket>::const_iterator it;
	for (it = rhs.getSockets().begin(); it != rhs.getSockets().end(); it++) {
		o << *it;
	}
	o << "************ end sockets ************" << std::endl;
	return o;
}
#endif
