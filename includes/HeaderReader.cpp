/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderReader.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 12:11:51 by aaqlzim           #+#    #+#             */
/*   Updated: 2021/06/08 17:23:14 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeaderReader.hpp"

// HeaderReader::HeaderReader(struct s_server *serv) {
// 	this->_serv = new std::vector<struct s_server *>;
// 	this->_server = new struct s_server;

// 	this->_server = serv;
// 	_serv->push_back(serv);
// }

// HeaderReader::~HeaderReader(void) {
// 	if (_serv)
// 	{
// 		_serv->clear();
// 		delete _serv;
// 	}
// }

// HeaderReader& HeaderReader::operator=(const HeaderReader& rhs) {
// 	if (this != &rhs)
// 	{
// 		_serv->clear();
// 		delete [] _serv;
// 		int i = 0;
// 		for (std::vector<struct s_server *>::iterator it = rhs._serv->begin();
// 		it != rhs._serv->end(); it++, i++)
// 		{
// 			this->_serv->push_back(*it);
// 			// this->_serv[i] = rhs._serv[i];
// 		}
// 	}
// 	return (*this);
// }

// HeaderReader::HeaderReader(const HeaderReader& rhs) {
// 	*this = rhs;
// }

// void HeaderReader::readConnection() {
// 	return ;
// }

HeaderReader::HeaderReader(int fd)
{
	this->_fd = fd;
}

std::string		HeaderReader::_readData()
{
	char	buff[MAX];
	size_t	r;

	std::memset((char *)buff, 0, MAX);
	r = read(this->_fd, buff, MAX);
	buff[r] = '\0';
	this->_r = r;
	this->_retbuff = strdup(buff);
	return (std::string(buff));
}

int		HeaderReader::_parseData()
{
	std::string _noSpace = "";

	for (size_t i = 0; i < this->_buff.size(); i++)
	{
		if (_buff[i] != ' ')
			_noSpace += _buff[i];
		else
		{
			_read.push_back(_noSpace);
			_noSpace = "";
		}
	}
	_read.push_back(_noSpace);
	return (1);
}

int		HeaderReader::_checkMethod()
{
	std::string method[3] = {"GET", "POST", "DELETE"};
	int j;
	std::vector<std::string>::iterator it;

	if (_read.size() != 3 && _read.size() < 2)
		return (0);
	it = _read.begin();
	for (int i = 0; i < 3; i++)
	{
		if (*it != method[i])
			j++;
		else
			_method = method[i];
	}
	if (j == 3)
		return (0);
	return 1;
}

int		HeaderReader::_checkUri()
{
	std::vector<std::string>::iterator it;

	if (_read.size() == 3 || _read.size() == 2)
	{
		it = _read.begin();
		it++;
		this->_uri = *it;
		return (1);
	}
	return (0);
}

int		HeaderReader::_checkProtocol()
{
	std::vector<std::string>::iterator it;
	
	if (this->_read.size() == 3)
	{
		it = _read.begin();
		it++;
		it++;
		this->_protocol = *it;
		return (1);
	}
	return (0);
}