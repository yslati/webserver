#ifndef HTTP_SERVER_H
# define HTTP_SERVER_H
# include <iostream>
class HttpServer {
	public:
		HttpServer() {}
		HttpServer(int port, std::string server_name) {
			_port = port;
			_server_name = server_name;
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

		// ----------------------------GETTERS--------------------------
		std::string const& getServerName() const {
			return _server_name;
		}

		int const& getPort() const {
			return _port;
		}
		// -----------------------------SETTERS-------------------------
		void	setServerName(std::string const& x) {
			_server_name = x;
		}

		void	setPort(int const& x) {
			_port = x;
		}
		// ----------------------------OVERLOADS-----------------------
	private:
		std::string _server_name;
		int _port;
};
std::ostream& operator<<(std::ostream& o, HttpServer const& rhs) {
	o << "httpserver:";
	o << std::endl << "Port: " << rhs.getPort();
	o << std::endl << "ServerName: " << rhs.getServerName();
	o << std::endl;
	return o;
}
#endif
