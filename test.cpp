#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>

int main()
{
    // std::string t = "Content-Type: text/html; charset=UTF-8";
    // std::string b = "Content-Type: multipart/form-data; boundary=---------------------------974767299852498929531610575";
    // std::string k = "";

    // std::string g = t.substr(t.find_first_of(':') + 2, t.find_first_of(';') - 14);
    // std::cout << "f0 = " << t.find_first_of(':') + 2 << std::endl;
    // std::cout << "f = " << t.find_first_of(';') << std::endl;
    // std::cout << b.substr(b.find("boundary=") + 9) << std::endl;
    // k = k.append("--").append(b.substr(b.find("boundary=") + 9));
    // std::cout << k << std::endl;
    // std::cout << b << std::endl;

    char buff[1024];
    getcwd(buff, sizeof(buff));

    std::string dir = buff;

    dir.append("/includes/_dhtml/index.html");
    std::fstream readFile(dir);
    std::string _line;
    std::string body = "";

    while (getline(readFile, _line))
        body.append(_line).append("\n");
    
    std::cout << body;
}