#ifndef PARSING_H
#define PARSING_H

#include "../includes/HttpServer.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

class pars {
	private:
		std::vector<std::string>	_conf;
		std::vector<int>			_servBegin;
		std::vector<int>			_servEnd;
		HttpServer			_httpServers;
	public:
		pars(std::string fileName);
		~pars();

		void	checkServer();
		void	parsServer(int i);
		// void	raseValue();

		
};

// std::ostream& operator<<(std::ostream& o, HttpServer const& rhs) {
// 	o << "httpserver:";
// 	o << std::endl << "Port: " << rhs.getPort();
// 	o << std::endl << "ServerName: " << rhs.getServerName();
// 	o << std::endl;
// 	return o;
// }

#endif