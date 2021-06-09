#include "HttpServer.hpp"
#include "Server.hpp"

int	main() {
	HttpServer s1(8080, "hello");
	HttpServer s2(8080, "hello_guys");
	HttpServer s3(80, "hello_guys");
	Server& server = Server::getInstance();	
	server.addHttpServer(s1);
	server.addHttpServer(s2);
	server.addHttpServer(s3);
	server.socketsCreate();
	server.socketsBind();
	server.socketsListen();
	server.acceptConnections();
	// std::cout << server;
	return (0);
}

