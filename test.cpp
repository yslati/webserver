#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>

struct Arg {
    std::string Cdisp;
    std::string Ctype;
    std::string Ftype;
};

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

    /*char buff[1024];
    getcwd(buff, sizeof(buff));

    std::string dir = buff;

    dir.append("/includes/_dhtml/index.html");
    std::fstream readFile(dir);
    std::string _line;
    std::string body = "";

    while (getline(readFile, _line))
        body.append(_line).append("\n");
    
    std::cout << body;*/
    // std::vector<struct Arg> v;

    // struct Arg f;
    // struct Arg f1;

    // f.Cdisp = "Content-Decposition: text/plain; charset=ascii";
    // f.Ctype = "Content-Type: text/html";
    // f.Ftype = "form-data";

    // f1.Cdisp = "Content-Decposition: text/html; charset=utf-8";
    // f1.Ctype = "Content-Type: image/png";
    // f1.Ftype = "json";
    // v.push_back(f);
    // v.push_back(f1);

    // std::vector<struct Arg>::iterator it = v.begin();

    // for (; it != v.end(); it++)
    // {
    //     std::cout << "Cdisp = " << it->Cdisp << "\n";
    // }

    // std::string t = "";
    // t += "--------------------------eb32ead89fa23a33\r\n";
    // t += "Content-Disposition: form-data; name=\"fname\"\r\n";
    // t += "\r\n";
    // t += "ayoub\r\n";
    // // t += "--------------------------eb32ead89fa23a33--\r\n";
    // t += "--------------------------eb32ead89fa23a33\r\n";
    // t += "Content-Disposition: form-data; filename=\"fname.txt\"\r\n";
    // t += "\r\n";
    // t += "ayoub hjshjkdhsk\r\n";
    // t += "--------------------------eb32ead89fa23a33--\r\n";

    // std::cout << "t.length() = " << t.length() << std::endl;

    std::string _Cdisp = "form-data; name=\"myfile\"; filename=\"commanf.txt\"";
    // std::string p = g.substr(0, g.find("filename") + 11);
    std::string type = _Cdisp.substr(_Cdisp.find("filename=\"") + 10,
		_Cdisp.length() - _Cdisp.substr(0, _Cdisp.find("filename") + 11).length());
    // std::cout << "size = " << g.length() - p.length() << std::endl;
    // std::cout << "size = " << p << std::endl;
    std::cout << type << std::endl;
}