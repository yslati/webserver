#ifndef SERVER_SOCKET
# define SERVER_SOCKET
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>

class ServerSocket {
	public:
		ServerSocket() {}
		~ServerSocket() {
			if (_fd != -1) {
				close(_fd);
			}
		}
		ServerSocket(ServerSocket const& src) {
			*this = src;
		}
		ServerSocket& operator=(ServerSocket const& rhs) {
			if (this != &rhs) {
				_fd = rhs._fd;
				_port = rhs._port;
			}
			return *this;
		}
		ServerSocket(int port) {
			_port = port;
		}
		bool operator==(ServerSocket const& rhs) {
			if (_port == rhs._port) {
				return true;
			}
			return false;
		}
		
		int const& getPort() const {
			return _port;
		}
		
		int const& getFd() const {
			return _fd;
		}

		void socketCreate() {
			_fd = socket(AF_INET, SOCK_STREAM, 0);
			if (_fd == -1) {
				std::cerr << "Socket failed" << std::endl;
			}
		}

		void socketBind() {
			if (_fd != -1) {
				sockaddr_in addr;
				addr.sin_family = AF_INET;
				addr.sin_addr.s_addr = INADDR_ANY;
			}
		}
	private:
		int _fd;
		int _port;
};
std::ostream& operator<<(std::ostream& o, ServerSocket const& rhs) {
	o << "Socket:";
	o << std::endl << "Fd :" << rhs.getFd();
	o << std::endl << "Port: " << rhs.getPort();
	o << std::endl;
	return o;
}
#endif
