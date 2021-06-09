#include "ErrorPage.hpp"

ErrorPage::ErrorPage(int code, std::string path) {
    _statusCode = code;
    _path = path;
}

void	ErrorPage::checkVal() const {
    std::cout << "Code: " << _statusCode << "\tPath: " << _path << std::endl;
}