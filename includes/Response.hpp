#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Request.hpp"
# include <stdexcept>
# include <unistd.h>
# include <fstream>
# include <map>
# include <sys/stat.h>
# include <cstdio>
# include <dirent.h>
# include <sys/types.h>
# include "../server/Location.hpp"

# define S_OK 200
# define S_MOVED_PERM 301
# define S_TEMP_REDIR 307
# define S_BAD_REQ 400
# define S_FORBIDDEN 403
# define S_NOT_FOUND 404
# define S_METHOD_NOT_ALLOWED 405
# define S_LENGTH_REQUIRED 411
# define S_PAY_LOAD_TOO_LARGE 413
# define S_URI_TOO_LONG 414
# define S_UNSUPPORTED_MEDIA_TYPE 415
# define S_INTERNAL_SERVER_ERROR 500
# define S_NOT_IMPLEMENTED 501
# define S_BAD_GATEWAY 502
# define S_GATEWAY_TIMEOUT 504
# define S_HTTP_VERSION_NOT_SUPPORTED 505
// AND SOME OTHER STATUS

// DEBUG MODE
# define DEBUG_MODE 1

class Response {
    private:
        Request _request;
        Location _location;
        size_t  _status;
        std::string _body;
        std::string _ResponseContent;
        std::map<int, std::string> _stResp;
    public:
        std::map<std::string, std::string> _headers;
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
        void _setLocation(Location& location);
        Location _getLocation() const;
        void _startResponse();
        void _applyMethod();
        std::string _getResContent() const;
        void _makeStatus();
        int _checkPermission(std::string path, int mode);
        std::string _getFilePath(std::string& uri);
        std::string _getErrorPagePath(std::string filename);
        std::string _getFileNameFromUri(std::string uri);
        std::string _getDir(void);
        std::string _generateHtmlTemplate();
        std::string _getHrefLink(std::string& dirname);
        void		_applyAutoIndexing();
		int			_isDir(std::string& dirname);
        // GET METHOD member
        void _applyGetMethod();
        void _readFile(const std::string& file);
        // POST METHOD member
        void _applyPostMethod();
        std::string _getFileNameFromDisp(std::string disp);
        // DELETE METHOD member
        void _applyDeleteMethod();
        void _handleError();
		void _generateErrorPage();
        void _deleteFile(std::string _file) {}

};

#endif