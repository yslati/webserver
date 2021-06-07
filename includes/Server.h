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

		// polymorph socketBind
		void socketsBind() {
			std::vector<ServerSocket>::iterator it;
			for (it = _sockets.begin(); it != _sockets.end(); it++) {
				it->socketBind();
			}	
		}

		// polymorph listen
		void socketsListen() {
			std::vector<ServerSocket>::iterator it;
			for (it = _sockets.begin(); it != _sockets.end(); it++) {
				it->socketListen();
			}	
		}

		// accepte connections
		void acceptConnections() {
			std::vector<ServerSocket>::iterator it;
			FD_ZERO(&master_set);
			addServerSocketsToSet();
			while (true) {
				worker_set = master_set;
				std::cout << "Select Waiting" << std::endl;
				if (select(FD_SETSIZE, &worker_set, NULL, NULL, NULL) < 0) {
					std::cerr << "Select failed" << std::endl;
					continue;
				}
				for (int i = 0; i < FD_SETSIZE; i++) {
					// if ready to read or accept a socket
					if (FD_ISSET(i, &worker_set)) {
						if ((it = isServerSocket(i)) != _sockets.end()) {
							int conn = it->acceptConnection();
							FD_SET(conn, &master_set);
						} else {
							std::cout << "Connection should we read from" << std::endl;
							// TODO read the request
							// depend on a strategy
							// if Content-Encoding chunked
							// ChunkedStrategy
							// else
							// Default Strategy
							FD_CLR(i, &master_set);
						}
					}
				}
			}
		}
	private:
		Server() {}
		static Server* _instance;
		std::vector<HttpServer> _http_servers;
		std::vector<ServerSocket> _sockets;
		fd_set master_set;
		fd_set worker_set;

		// add socket
		void addSocket(ServerSocket const& ss) {
			std::vector<ServerSocket>::iterator it;
			it = std::find(_sockets.begin(), _sockets.end(), ss);
			if (it == _sockets.end()) {
				_sockets.push_back(ss);
			}
		}

		void addServerSocketsToSet() {
			std::vector<ServerSocket>::iterator it;
			for (it = _sockets.begin(); it != _sockets.end(); it++) {
				if (it->getFd() != -1) {
					FD_SET(it->getFd(), &master_set);
				}
			}
		}

		std::vector<ServerSocket>::iterator isServerSocket(int fd) {
			std::vector<ServerSocket>::iterator it;
			for (it = _sockets.begin(); it != _sockets.end(); it++) {
				if (it->getFd() == fd) {
					return it;
				}
			}
			return it;
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
