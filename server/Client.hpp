#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <netinet/in.h>
# include <sys/socket.h>
# include <iostream>
# include <fcntl.h>
# include <poll.h>
# include <unistd.h>
# include "HttpServer.hpp"
# include "../includes/Request.hpp"
# include "../includes/Response.hpp"

class Client {
    public:
        Client(int server_fd);

        int getConnection();
        int readConnection();
	void writeConnection();
        std::string getContent() const;
        bool getReady();
        void setReady(bool x);

        struct pollfd getPfd();
        std::string getResponseContent();
        void _handleRequest(std::vector<HttpServer>::iterator it);
        void _handleResponse(Request req);
	~Client();
    private:
	int sended;
        int _conn;
        struct sockaddr_in addr;
        bool _ready;
	std::string responseContent;
        std::string content;
        struct pollfd pfd;
};
#endif
