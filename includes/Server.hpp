#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <vector>
# include "HttpServer.hpp"
# include "ServerSocket.hpp"
# include "HeaderReader.hpp"
# include <cstdlib>
# include <map>
# include <algorithm>
# include "Response.hpp"
# include "Request.hpp"

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
			char buffer[MAX];
			Request req;
			FD_ZERO(&master_set);
			addServerSocketsToSet();
			std::map<int, std::string> rmap;
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
							// std::memset((char *)buffer, 0, MAX);
							// size_t r = read(i, buffer, MAX);
							int r = recv(i, buffer, sizeof(buffer), 0);
							buffer[r] = '\0';
							req._parseIncomingRequest(std::string(buffer));
							// HeaderReader _headerReader(i);
							// _headerReader._readData();
							// _headerReader._parseData();
							write(1, buffer, sizeof(buffer));
							// send(1, buffer, sizeof(buffer), 0);
							// std::cout  << "buffer = " << buffer[r] << "\n";
							// write(1, _headerReader._retbuff, _headerReader._r);
							std::string str;
							str = "HTTP/1.1 200 OK\r\n";
							str += "Content-Type: text/html\r\n";
							str += "Content-Length: 11\r\n";
							str += "\r\n";
							str += "Hello World";
							std::pair<int, std::string> p(i, str);
							rmap.insert(p);
							FD_SET(i, &response_set);
							// FD_CLR(i, &master_set);
						}
					}
				}

				/*for (int i = 0; i < FD_SETSIZE; i++) {
					// Response res;
					// res._setRequest(req);
					// res._startResponse();
					// std::string content = res._getResContent();
					if (FD_ISSET(i, &response_set)) {
						std::map<int, std::string>::iterator it;
						it = rmap.find(i);
						if (it != rmap.end()) {
							write(i, it->second.c_str(), it->second.size());
						}
						FD_CLR(i, &response_set);
					}
				}*/
			}
		}
	private:
		Server() {}
		static Server* _instance;
		std::vector<HttpServer> _http_servers;
		std::vector<ServerSocket> _sockets;
		fd_set master_set;
		fd_set worker_set;
		fd_set response_set;

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
