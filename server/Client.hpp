#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <netinet/in.h>
# include <sys/socket.h>
# include <iostream>
# include <fcntl.h>

class Client {
    public:
        Client(int server_fd);

        int getConnection();
        bool readConnection();
        std::string getContent() const;
    private:
        int _conn;
        struct sockaddr_in addr;
        std::string content;
};
#endif