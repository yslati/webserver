#ifndef LOCATION_HPP
# define LOCATION_HPP
# include <iostream>
# include <vector>

class Location {
	public:
		Location() {
			_status_code = -1;
			_auto_index = false;
			_is_redirect = false;
		}

		void	setUri(std::string const& x) {	_uri = x; }
		void	setRoot(std::string const& x) {	_root = x; }
		void	setIndex(std::string const& x) {	_index = x; }
		void	setAllowedMethods(std::string x) {	_allowed_methods.push_back(x); }
		void	setAutoIndex(bool x) {	_auto_index = x; }
		void	setIsRedirect(bool x) {	_is_redirect = x; }
		void	setStatusCode(int x) {	_status_code = x; }
		void	setRedirectUrl(std::string const& x) {	_redirect_url = x; }


		void	checkVal() {
			std::cout << "URI: \t\t" << _uri << "\t\tRoot: \t\t" << _root << std::endl;
			std::cout << "index: \t\t" << _index << "\tAllowed Method: ";
			for (int i = 0; i < _allowed_methods.size(); i++)
				std::cout << _allowed_methods[i] << ", ";
			std::cout << std::endl;
			std::cout << "auto_index: \t" << std::boolalpha << _auto_index << "\t\tredirect: \t" << _is_redirect << std::endl;
			std::cout << "code: \t\t" << _status_code << "\t\tredirect_path: \t" << _redirect_url << std::endl;
			std::cout << "============================" << std::endl;
		}

	private:
		std::string _uri;
		std::string _root;
		std::string _index;
		std::vector<std::string> _allowed_methods;
		bool _auto_index;
		bool _is_redirect;
		int _status_code; // 301 302 redirect code
		std::string _redirect_url;
};

#endif