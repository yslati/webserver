#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <vector>
# include "HttpServer.hpp"
# include "ServerSocket.hpp"
# include <algorithm>
# include "RequestReader.hpp"

class Server {
	public:
		static Server& getInstance();

		void addHttpServer(HttpServer const& server);

		std::vector<ServerSocket> const& getSockets() const;

		std::vector<HttpServer> const& getHttpServers() const;

		// polymorph socketCreate
		void socketsCreate();

		void socketsBind();
		void socketsListen();

		void	acceptConnections();

		void addServerSocketsToSet();
		std::vector<ServerSocket>::iterator isServerSocket(int fd);
	private:
		Server();
		static Server* _instance;
		std::vector<HttpServer> _http_servers;
		std::vector<ServerSocket> _sockets;
		fd_set worker_set;
		fd_set master_set;
		fd_set response_set;

		// add socket
		void addSocket(ServerSocket const& ss);
};



// std::ostream& operator<<(std::ostream& o, Server const& rhs) {
// 	o << "Server:\n";
// 	o << "************ servers  ************" << std::endl;
// 	std::vector<HttpServer>::const_iterator its;
// 	for (its = rhs.getHttpServers().begin(); its != rhs.getHttpServers().end(); its++) {
// 		o << *its;
// 	}
// 	o << "************ end servers  ************" << std::endl;
// 	o << "************ sockets ************" << std::endl;
// 	std::vector<ServerSocket>::const_iterator it;
// 	for (it = rhs.getSockets().begin(); it != rhs.getSockets().end(); it++) {
// 		o << *it;
// 	}
// 	o << "************ end sockets ************" << std::endl;
// 	return o;
// }
#endif
