#include "HttpServer.hpp"
#include "Server.hpp"
#include "parsing.hpp"

int	main(int argc, char **argv) {
	if (argc < 2) 
		return 1;
	pars::pars(argv[1]);
	HttpServer s1(8080, "hello");
	// HttpServer s2(8080, "hello_guys");
	// HttpServer s3(80, "hello_guys");
	Server& server = Server::getInstance();	
	// server.addHttpServer(s1);
	// server.addHttpServer(s2);
	server.addHttpServer(s1);
	server.socketsCreate();
	server.socketsBind();
	server.socketsListen();
	server.acceptConnections();
	// std::cout << server;
	return (0);
}

