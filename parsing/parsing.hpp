#ifndef PARSING_H
#define PARSING_H

#include "HttpServer.hpp"
#include "Location.hpp"
#include "Server.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <cctype>
#include <sstream>
#include <exception>
#include <stdexcept>


class pars {
	private:
		std::vector<std::string>	_conf;
		std::vector<int>			_servBegin;
		std::vector<int>			_servEnd;
		
		Server						&_Servers;
		

		std::vector<std::string>	_split(std::string const &str, char sep);
		bool						_checkbool(std::string str);

	public:
		pars(std::string fileName);
		~pars();

		void	checkServer();
		void	parsServer(int i);
		int		parsLocation(int i, int end, HttpServer& srv);

};

#endif
