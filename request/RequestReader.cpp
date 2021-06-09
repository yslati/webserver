#include "RequestReader.hpp"

void RequestReader::readConnection(int connectionFd) {
    char buffer[1024];
    int ret;

    while ((ret = recv(connectionFd, buffer, 1023, 0)) >= 1023) {
        buffer[ret] = '\0';
        std::cout << buffer << std::endl;
    }

    if (ret == -1) {
        std::cerr << "recv error" << std::endl;
    } else {
        buffer[ret] = '\0';
        std::cout << buffer << std::endl;
    }
}