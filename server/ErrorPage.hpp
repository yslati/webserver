#ifndef ERROR_PAGE_HPP
# define ERROR_PAGE_HPP
# include <iostream>

class ErrorPage {
	public:
		ErrorPage(int code, std::string path);


		void	checkVal() const;

	private:
		int _statusCode;
		std::string _path;
};

#endif