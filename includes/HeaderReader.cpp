/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderReader.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 12:11:51 by aaqlzim           #+#    #+#             */
/*   Updated: 2021/06/07 13:08:22 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeaderReader.hpp"

HeaderReader::HeaderReader(struct s_server *serv) {
	this->_serv = new std::vector<struct s_server *>;
	this->_server = new struct s_server;

	this->_server = serv;
	_serv->push_back(serv);
}

HeaderReader::~HeaderReader(void) {
	if (_serv)
	{
		_serv->clear();
		delete _serv;
	}
}

HeaderReader& HeaderReader::operator=(const HeaderReader& rhs) {
	if (this != &rhs)
	{
		_serv->clear();
		delete [] _serv;
		int i = 0;
		for (std::vector<struct s_server *>::iterator it = rhs._serv->begin();
		it != rhs._serv->end(); it++, i++)
		{
			this->_serv->push_back(*it);
			// this->_serv[i] = rhs._serv[i];
		}
	}
	return (*this);
}

HeaderReader::HeaderReader(const HeaderReader& rhs) {
	*this = rhs;
}

void HeaderReader::readConnection() {
	return ;
}