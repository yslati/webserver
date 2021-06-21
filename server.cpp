# include <iostream>
# include <cstring>
# include <cstdlib>

// Network Lib
# include <poll.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/types.h>
// # include <sys/event.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>
# include <vector>

const int MAX = 1024;
const int PORT = 8080;

int main()
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    std::string hello = "Hello from server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "In socket\n";
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    std::memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        std::cerr << "In bind\n";
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        std::cerr << "In listen\n";
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            std::cerr << "In accept\n";
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        std::cout << buffer << std::endl;
        write(new_socket , hello.c_str() , hello.length());
        std::cout << "------------------Hello message sent-------------------\n";
        close(new_socket);
    }
}