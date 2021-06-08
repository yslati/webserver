#ifndef HTTP_SERVER_HPP
# define HTTP_SERVER_HPP
# include <iostream>
#include <vector>
# include "ErrorPage.hpp"

class HttpServer {
	public:
		HttpServer() {}
		HttpServer(int port, std::string server_name) {
			_port = port;
			_server_name = server_name;
			_host = "";
			_allowed_methods = "";
		}
		HttpServer(HttpServer const& src) {
			*this = src;
		}
		HttpServer& operator=(HttpServer const& rhs) {
			if (this != &rhs) {
				_port = rhs._port;
				_server_name = rhs._server_name;
			}
			return *this;
		}


		// ---------------------------CHECKERS--------------------------
		void	checkVal() const {
			// std::cout << "server_name: " << _server_name << "\tPort: " << _port << std::endl;
			// std::cout << "allowed_methods: " << _allowed_methods << "\thost: " << _host << std::endl;
			// std::cout << "============================" << std::endl;
		}

		// ----------------------------GETTERS--------------------------
		std::string const& getServerName() const {
			return _server_name;
		}

		std::string const& getHost() const {
			return _host;
		}

		std::string const& getAllowedMethods() const {
			return _allowed_methods;
		}

		int const& getPort() const {
			return _port;
		}
		// -----------------------------SETTERS-------------------------
		void	setServerName(std::string const& x) {
			_server_name = x;
		}

		void	setHost(std::string const& x) {
			_host = x;
		}

		void	setAllowedMethods(std::string const& x) {
			_allowed_methods = x;
		}

		void	setPort(int const& x) {
			_port = x;
		}
		// ----------------------------OVERLOADS-----------------------

		void	addErrorPage(int statusCode, std::string path) {
			ErrorPage ep(statusCode, path);
			_error_pages.push_back(ep);
		}
	
	private:
		std::string _server_name;
		int _port;
		std::vector<ErrorPage> _error_pages;
		std::string		_host;
		std::string		_allowed_methods;
};

// std::ostream& operator<<(std::ostream& o, HttpServer const& rhs);
#endif