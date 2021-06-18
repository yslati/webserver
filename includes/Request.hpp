#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <vector>
# include <algorithm>
# include <iostream>
# include <sstream>
# include <map>
# include <string>
// # include "../server/Client.hpp"
#include "../server/HttpServer.hpp"

class Request {
    public:
        struct ArgContent {
			std::string _Cdisp;
			std::string _Ctype;
			std::string _data;
        };
        Request();
        ~Request();
        int _fd;
        void _parseIncomingRequest(const std::string& _buffer);
        void _parseLine(const std::string& _line);
        const std::string& _getMethod() const;
        const std::string& _getUri() const;
        std::string _getProtocol() const;
        unsigned int _getContentLenght() const;
        const std::string& _getContentType() const;
        std::string _getHeaderContent(std::string _first);
        void _pushDataToArg(std::string _data);
        ArgContent _pushToArg(std::string _data);
        ArgContent _getArg(size_t i);
        std::vector<ArgContent> _getVecCont() const;
        void _printArg();
        bool _isPrefix(std::string& s1, std::string& s2);
        bool _matchBegin(std::string& _regex, std::string& _line);
        int _getContentLen();
        int _getError();
        std::string _getCgiUriFile() const { return "hey"; }
    private:
        bool            _isDone;
        std::string _Host;
        std::string _method;
        std::string _uri;
        std::string _protocol;
        std::string _Server;
        std::string _Ctype;
        std::string _Atype;
        std::string _Cdisp;
        unsigned int _Clen;
        std::string _Conn;
        std::vector<std::string> _parse;
        std::map<std::string, std::string> _rmap;
        std::vector<ArgContent> _aCont;
        bool            _isArg;
        std::string     _argBody;
        int _lenArg;
        std::string _boundary;
        std::string _body;
        std::vector<HttpServer>::iterator _it;
        int _error;
};

#endif