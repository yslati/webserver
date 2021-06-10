<<<<<<< HEAD
#include "./includes/HttpServer.hpp"
#include "./includes/Server.hpp"

int	main() {
	// std::string tmp = "<html>\n";
	// tmp += "<head>\n";
	// tmp += "	<title>405 Not Allowed</title>\n";
	// tmp += "</head>\n";
	// tmp += "<body>\n";
	// tmp += "	<center>\n";
	// tmp += "		<h1>405 Not Allowed</h1>\n";
	// tmp += "	</center>\n";
	// tmp += "	<hr>\n";
	// tmp += "	<center>nginx/1.21.0</center>\n";
	// tmp += "</body>\n";
	// tmp += "</html>\n";

	// std::cout << "body len = " << tmp.length() << std::endl;

=======
#include "HttpServer.hpp"
#include "Server.hpp"
#include "parsing.hpp"

int	main(int argc, char **argv) {
	if (argc < 2) 
		return 1;
	pars::pars(argv[1]);
>>>>>>> ba23578ae19ed6898d0bafb106edb9bed0dc5405
	HttpServer s1(8080, "hello");
	// HttpServer s2(8080, "hello_guys");
	// HttpServer s3(80, "hello_guys");
	Server& server = Server::getInstance();	
	// server.addHttpServer(s1);
	// server.addHttpServer(s2);
	server.addHttpServer(s1);
<<<<<<< HEAD
	// server.addHttpServer(s2);
	// server.addHttpServer(s3);
	server.socketsCreate();
	server.socketsBind();
	server.socketsListen();
	server.acceptIncomingConnections();
	std::cout << "OK";
=======
	server.socketsCreate();
	server.socketsBind();
	server.socketsListen();
	server.acceptConnections();
	// std::cout << server;
>>>>>>> ba23578ae19ed6898d0bafb106edb9bed0dc5405
	return (0);
}

// std::cout << it->getPort() << std::endl;
// int conn = it->acceptConnection();
// char buffer[1024];
// int ret;
// std::string reqContent;

// while ((ret = read(conn, buffer, sizeof(buffer) - 1)) == 1023) {
// 		std::cout << ret << std::endl;
// 		buffer[ret] = '\0';
// 		std::string tmp = "";
// 		tmp.assign(buffer);
// 		reqContent += tmp;
// }
            
// std::string tmp = "";
// tmp.assign(buffer);
// reqContent += tmp;

// std::cout << reqContent << std::endl;
// std::cout << "End" << std::endl;
// close(conn);