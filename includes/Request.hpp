#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <vector>
# include <algorithm>
# include <iostream>
# include <sstream>

class Request {
    private:
        std::string _Host;
        std::string _method;
        std::string _uri;
        std::string _protocol;
        std::string _Server;
        std::string _Date;
        std::string _Ctype;
        std::string _Atype;
        std::string _Cdisp;
        unsigned int _Clen;
        std::string _Lmodified;
        std::string _Conn;
        std::string _Etag;
        std::string _Aranges;
        std::string _boundary;
        std::vector<std::string> _parse;
    public:
        Request();
        ~Request();

        void _parseIncomingRequest(const std::string& _buffer);
        void _parseLine(const std::string& _line);
        const std::string& _getMethod() const;
        const std::string& _getUri() const;
        const std::string& _getProtocol() const;
        unsigned int _getContentLenght() const;
        const std::string& _getContentType() const;
};

#endif