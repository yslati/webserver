#ifndef LOCATION_HPP
# define LOCATION_HPP
# include <iostream>
# include <vector>

class Location {
	public:
		Location() {
		}

		void	setUri(std::string const& x) {
			_uri = x;
		}
		void	setRoot(std::string const& x) {
			_root = x;
		}
		void	setIndex(std::string const& x) {
			_index = x;
		}
		void	setAllowedMethods(std::string x) {
			_allowed_methods.push_back(x);
		}
		void	setAutoIndex(bool x) {
			_auto_index = x;
		}
		void	setIsRedirect(bool x) {
			_is_redirect = x;
		}
		void	setRedirectUrl(std::string const& x) {
			_redirect_url = x;
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