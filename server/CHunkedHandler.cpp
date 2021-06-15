#include "ChunkedHandler.hpp"
#include "HexConvertor.hpp"
#include <sstream>

void ChunkedHandler::handle(std::string& content) {
    if (content.find("Transfer-Encoding") == std::string::npos)
        return;
    size_t i = content.find("\r\n\r\n");
    std::string tmp = content.substr(i + 4);
    std::cout << "CONTENT" << std::endl;
    std::cout << tmp;
    std::istringstream iss(tmp);
    std::string _line;
    int l = 0;
    bool is_number = true;
    int nbr = 0;
    std::string chunk = "";
    std::string newContent;
    // converter
    mlib::HexConvertor *c = new mlib::HexConvertor();
    while (std::getline(iss, _line))
    {
        std::cout << l << " " << _line << std::endl;
        if (is_number) {
            nbr = c->to_decimal(_line.substr(0, _line.size() - 1));
            is_number = false;
            if (nbr == 0) {
                break;
            }
        } else {
            _line.append("\n");
            chunk.append(_line.substr(((nbr <= _line.size()) ? nbr : _line.size())));
            if (chunk.size() == nbr) {
                is_number = true;
                newContent.append(chunk);
                chunk = "";
            }
        }
        l++;
    }
    std::cout << newContent;
    std::cout << "ENDOFCONTENT" << std::endl;
}