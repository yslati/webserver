#include "HttpServer.h"
#include "Server.h"

int	main() {
	HttpServer s1(8080, "hello");
	HttpServer s2(8080, "hello_guys");
	HttpServer s3(80, "hello_guys");
	Server& server = Server::getInstance();	
	server.addHttpServer(s1);
	server.addHttpServer(s2);
	server.addHttpServer(s3);
	server.socketsCreate();
	std::cout << server;
	return (0);
}

