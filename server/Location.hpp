#ifndef LOCATION_HPP
# define LOCATION_HPP
# include <iostream>
# include <vector>


class Location {
	public:
		Location();
		Location(Location const& rhs);
		~Location();

		void			setUri(std::string const& x);
		void			setRoot(std::string const& x);
		void			setIndex(std::string const& x);
		void			setAllowedMethods(std::vector<std::string> x);
		void			setAutoIndex(bool x);
		void			setIsRedirect(bool x);
		void			setStatusCode(int x);
		void			setRedirectUrl(std::string const& x);
		void			setIsUploadEnable(bool x);
		void			setUploadDir(std::string dir);

		bool			getAutoIndex() const;
		bool			getIsRedirect() const;
		bool			getIsUploadEnable() const;
		std::string 	getUploadDir() const;
		int				getStatusCode() const;
		std::string		getUri() const;
		std::string		getRoot() const;
		std::string		getIndex() const;
		std::string		getRedirectUrl() const;

		std::vector<std::string>	getAllowedMethod() const;

		void	checkVal();

	private:
		bool _isUploadEnable;
		std::string _uploadDir;
		std::string _uri;
		std::string _root;
		std::string _index;
		std::vector<std::string> _allowed_methods;
		bool _auto_index;
		bool _is_redirect;
		std::string _redirect_url;
		int _status_code; // 301 302 redirect code
};

#endif
