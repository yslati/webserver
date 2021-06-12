#include "Client.hpp"

Client::Client(int server_fd) {
    int len = sizeof(addr);
    _ready = false;
    _conn = accept(server_fd, (struct sockaddr*)&addr, (socklen_t*)&len);
    if (_conn < 0) {
        throw std::runtime_error("accept failed");
    }
    fcntl(_conn, F_SETFL, O_NONBLOCK);
    pfd.events = POLLIN;
    pfd.fd = _conn;
}

bool Client::getReady() {
        return this->_ready;
}

void Client::setReady(bool x) {
        if (x)
                pfd.events = POLLOUT;
        else
                pfd.events = POLLIN;
        this->_ready = x;
}

int Client::getConnection() {
    return this->_conn;
}

struct pollfd Client::getPfd() {
        return this->pfd;
}

int checkEnd(const std::string& str, const std::string& end)
{
        size_t  i = str.size();
        size_t  j = end.size();

        while (j > 0)
        {
                i--;
                j--;
                if (i < 0 || str[i] != end[j])
                        return (1);
        }
        return (0);
}

int Client::readConnection() {
    char buffer[1028];
    int r = recv(_conn, buffer, 128, 0);
//     return 0;
    std::cout << r << std::endl;
    if (r == -1) {
            return 1;
    }
    if (r == 0) {
            throw std::runtime_error("Closed");
    }
    else {
            std::string tmp;
            buffer[r] = '\0';
            tmp.assign(buffer);
            content += tmp;
            std::cout << tmp;
            // std::cout << i << " " << content.size() << std::endl;
            // std::cout << "ENDOK" << std::endl;
            size_t j = content.find("\r\n\r\n", 0);

            // check if there is a \r\n\r\n
            if (j != std::string::npos)
            {
                    // if there isnt a content length
                    if (content.find("Content-Length: ") == std::string::npos) {
                            // check if there is chunked
                            if (content.find("Transfer-Encoding: chunked") != std::string::npos) {
                                    if (checkEnd(content, "0\r\n\r\n") == 0)
                                            return (0);
                                    else
                                            return (1);
                            }
                            else
                                    return (0);
                    }
                    size_t  len = std::atoi(content.substr(content.find("Content-Length: ") + 16, 10).c_str());
                //     size_t pos = content.find("\r\n\r\n");
                    if (content.size() >= len + j + 4)
                    {
                            std::cout << "kahkjfdhsa" << std::endl;
                            return (0);
                    }
                    else
                            return (1);
            }
    }
    return (1);
}

std::string Client::getContent() const {
    return this->content;
}

// POST /filename HTTP/1.1
// 