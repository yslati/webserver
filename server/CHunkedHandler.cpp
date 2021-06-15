#include "ChunkedHandler.hpp"
#include "HexConvertor.hpp"
#include <sstream>

void ChunkedHandler::handle(std::string& content) {
    if (content.find("Transfer-Encoding") == std::string::npos)
        return;
    size_t i = content.find("\r\n\r\n");
    std::string tmp = content.substr(i + 4);
    std::cout << "CONTENT" << std::endl;
    // std::cout << tmp;
    std::istringstream iss(tmp);
    std::string _line;
    int l = 0;
    bool is_number = true;
    int nbr = 0;
    std::string chunk = "";
    std::string newContent = "";
    // converter
    mlib::HexConvertor *c = new mlib::HexConvertor();
    while (std::getline(iss, _line)) {
        // if (is_number) {
        //     if (_line.compare("0") == 0)
        //         break;
        //     nbr = c->to_decimal(_line.substr(0, _line.size() - 1));
        //     is_number = false;
        //     chunk = "";
        // } else {
        //     chunk += _line.substr(0, nbr - chunk.size());
        //     if (chunk.size() == nbr) {
        //         newContent += chunk;
        //         is_number = true;
        //     }
        // }
        if (is_number) {
            std::cout << c->to_decimal(_line.substr(0, _line.size() - 1)) << std::endl;
            is_number = false;
        } else {
            is_number = true;
        }
    }
    
    std::cout << newContent;
    std::cout << "ENDOFCONTENT" << std::endl;
}