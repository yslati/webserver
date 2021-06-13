// #include "HttpServer.hpp"
#include "Server.hpp"
#include "parsing.hpp"

int		main()
{
	try {
		pars ser("./parsing/file.conf");
		Server& srv = Server::getInstance();
		srv.acceptConnections();
	} catch (char const *e) {
		std::cout << e << std::endl;
		return (1);
	}
	return (0);	
}