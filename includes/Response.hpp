#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Request.hpp"
# include <stdexcept>

class Response {
    private:
        Request _request;
        size_t  _status;
        std::string _body;
        std::string _ResponseContent;
    public:
        class MethodNotFound: public std::exception
        {
            public:
                virtual const char* what() const throw() {
                    return "Not Implemented";
                } 
        };
        Response();
        ~Response();

        // THE RESPONSE CODE GOES HERE
        void _setRequest(Request& req);
        void _startResponse();
        void _applyMethod();
        const std::string& _getResContent() const;

        // GET METHOD member
        const std::string& _getFilePath(const std::string& uri) const;
        void _readFile(const std::string& path);
        // POST METHOD member
        // DELETE METHOD member
};

#endif