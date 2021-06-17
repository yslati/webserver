#include "Location.hpp"

Location::Location() {
    _uri = "/";
    _status_code = -1;
    _auto_index = false;
    _is_redirect = false;
}

Location::Location(Location const & rhs)
{
    *this = rhs;
}

Location::~Location() {
    
}

void		Location::setUri(std::string const& x) {	_uri = x; }
void		Location::setRoot(std::string const& x) {	_root = x; }
void		Location::setIndex(std::string const& x) {	_index = x; }
void		Location::setAllowedMethods(std::vector<std::string> x) {
    for (int i = 0; i < x.size(); i++) {
        if (x[i] != "GET" && x[i] != "POST" && x[i] != "DELETE")
            throw "allowed method not acceptable";
        _allowed_methods.push_back(x[i]);
    }
}
void		Location::setAutoIndex(bool x) {	_auto_index = x; }
void		Location::setIsRedirect(bool x) {	_is_redirect = x; }
void		Location::setStatusCode(int x) {	_status_code = x; }
void		Location::setRedirectUrl(std::string const& x) {	_redirect_url = x; }

bool		Location::getAutoIndex() const { return (_auto_index); }
bool		Location::getIsRedirect() const { return (_is_redirect); }
int			Location::getStatusCode() const { return (_status_code); }
std::string	Location::getUri() const { return (_uri); }
std::string	Location::getRoot() const { return (_root); }
std::string	Location::getIndex() const { return (_index); }
std::string	Location::getRedirectUrl() const { return (_redirect_url); }
std::vector<std::string>	Location::getAllowedMethod() const { return (_allowed_methods); }


void		Location::checkVal() {
    std::cout << "URI: \t\t" << _uri << "\t\tRoot: \t\t" << _root << std::endl;
    std::cout << "index: \t\t" << _index << "\tAllowed Method: ";
    for (int i = 0; i < _allowed_methods.size(); i++)
        std::cout << _allowed_methods[i] << " ";
    std::cout << std::endl;
    std::cout << "auto_index: \t" << std::boolalpha << _auto_index << "\t\tredirect: \t" << _is_redirect << std::endl;
    std::cout << "code: \t\t" << _status_code << "\t\tredirect_path: \t" << _redirect_url << std::endl;
    std::cout << "============================" << std::endl;
}
