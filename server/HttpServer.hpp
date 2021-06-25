#ifndef HTTP_SERVER_HPP
# define HTTP_SERVER_HPP
# include <iostream>
#include <vector>
#include <map>
#include <iterator>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <arpa/inet.h>
# include "Location.hpp"
# include <fcntl.h>

#define log std::cout << 
#define line << std::endl

class HttpServer {
	public:
		HttpServer();
		HttpServer(HttpServer const& src);
		HttpServer& operator=(HttpServer const& rhs);


		// ----------------------------GETTERS--------------------------
		std::string const&					getHost() const;
		std::vector<int> const&				getPort();
		std::string const&					getServerName() const;
		std::string const&					getRoot() const;

		std::vector<std::string> const&		getAllowedMethods() const;
		int const&							getMaxBodySize() const;
		int getFd() const;

		std::vector<Location>				getLocations();
		std::string							_getErrorPages(int st);
		// -----------------------------SETTERS-------------------------
		void								setHost(std::string const& x);
		void								setPort(int const& x);
		void								setServerName(std::string const& x);
		void								setRoot(std::string const& x);

		void								setAllowedMethods(std::vector<std::string> x);
		void								setMaxBodySize(int const& x);

		void								addLocation(Location const& loc);

		// ----------------------------OVERLOADS-----------------------

		void								addErrorPage(int statusCode, std::string path);
		void								start_listen();


	private:
		std::vector<int>			_port;
		int							_maxBodySize;
		std::string					_server_name;
		std::string					_host;
		std::string					_root;
		std::vector<std::string>	_allowed_methods;
		std::vector<Location> 		_locations;
		std::map<int, std::string>	_errors;
		int 						_stcode;
		int							_fd;
		struct sockaddr_in			_addr;

};

#endif
