#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <netinet/in.h>
# include <sys/socket.h>
# include <iostream>
# include <fcntl.h>
# include <poll.h>

class Client {
    public:
        Client(int server_fd);

        int getConnection();
        int readConnection();
        std::string getContent() const;
        bool getReady();
        void setReady(bool x);

        struct pollfd getPfd();
    private:
        int _conn;
        struct sockaddr_in addr;
        bool _ready;
        std::string content;
        struct pollfd pfd;
};
#endif