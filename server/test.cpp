#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include "ServerSocket.hpp"
#include <vector>
#include <fcntl.h>
#include <set>

typedef struct pollfd pfd;

std::string readConnection(int fd) {
    int ret;
    char buffer[129];
    std::string content;

    while (true)
    {
        ret = recv(fd, buffer, 128, 0);
        if (ret == 0) {
            throw std::runtime_error("Closed");
        }
        std::string tmp;
        buffer[ret] = '\0';
        tmp.assign(buffer);
        content += tmp;
        if (ret < 128)
            break;
    }
    return content;
}

void    writeToConnection(int fd) {
    std::string content = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 11\r\nConnection: keep-alive\r\n\r\nhello world";
    int ret = send(fd, content.c_str(), content.size(), 0);
    std::cout << ret;
}

int     main() {
    ServerSocket s(1337);
    s.socketCreate();
    s.socketBind();
    s.socketListen();

    std::vector<struct pollfd> fds;
    fds.push_back((struct pollfd){s.getFd(), POLLIN});


    while (true)
    {
        std::vector<struct pollfd> tmp;
        std::set<int> toRemove;
    
        std::cout << "Open fds keep alive" << std::endl;
        for (size_t i = 0; i < fds.size(); i++)
        {
            std::cout << fds[i].fd << " ";
        }
        std::cout << std::endl;
        
        int n = poll(&fds[0], fds.size(), 1000);
        if (n == 0) {
            continue;
        }
        std::cout << "Poll" << std::endl;
        for (int i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == s.getFd()) {
                    int conn = s.acceptConnection();
                    tmp.push_back((struct pollfd){ conn, POLLIN });
                } else {
                    try
                    {
                        // read connection
                        // fds[i].fd // content
                        std::string content = readConnection(fds[i].fd);
                        std::cout << content << std::endl;
                        fds[i].events = POLLOUT | POLLIN;
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                        toRemove.insert(i);
                    }
                }
            } else if (fds[i].revents & POLLOUT) {
                
                try
                {
                    // write to Connection
                    writeToConnection(fds[i].fd);
                    fds[i].events = POLLIN;
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                    toRemove.insert(i);
                }
            } else if (fds[i].revents & POLLHUP){
                toRemove.insert(i);
            }
        }
        std::set<int>::iterator it;
        for (it = toRemove.begin(); it != toRemove.end(); it++)
        {
            if (fds.begin() + *it != fds.end()) {
                close((fds.begin() + *it)->fd);
                fds.erase(fds.begin() + *it);
            }
        }
        fds.insert(fds.end(), tmp.begin(), tmp.end());
    }
}
