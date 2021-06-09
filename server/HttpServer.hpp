#ifndef HTTP_SERVER_HPP
# define HTTP_SERVER_HPP
# include <iostream>
#include <vector>
#include <map>
#include <iterator>
# include "ErrorPage.hpp"

#define log std::cout << 
#define line << std::endl

class HttpServer {
	public:
		HttpServer();
		HttpServer(int port, std::string server_name);
		HttpServer(HttpServer const& src);

		HttpServer& operator=(HttpServer const& rhs);


		// ---------------------------CHECKERS--------------------------
		void	checkVal() const;

		// ----------------------------GETTERS--------------------------
		std::string const& getServerName() const;

		std::string const& getHost() const;

		std::vector<std::string> const& getAllowedMethods() const;

		int const& getPort() const;
		// -----------------------------SETTERS-------------------------
		void	setServerName(std::string const& x);

		void	setHost(std::string const& x);

		void	setAllowedMethods(std::string const& x);

		void	setPort(int const& x);
		// ----------------------------OVERLOADS-----------------------

		void	addErrorPage(int statusCode, std::string path);

	private:
		int							_port;
		std::string					_server_name;
		std::string					_host;
		// std::vector<ErrorPage>		_error_pages;
		std::map<int, std::string>	_errors;
		std::map<char,int> mymap;
		std::vector<std::string>	_allowed_methods;
};

// std::ostream& operator<<(std::ostream& o, HttpServer const& rhs);
#endif
