#include "./includes/HttpServer.h"
#include "./includes/Server.h"

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
	std::cout << "OK";
	return (0);
}

