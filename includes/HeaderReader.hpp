/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderReader.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 11:53:41 by aaqlzim           #+#    #+#             */
/*   Updated: 2021/06/08 17:22:52 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_READER_HPP
# define HEADER_READER_HPP

#include "IReader.hpp"
#include <map>

struct					s_client
{
	int					sock;
	long				valread;
	struct sockaddr_in	serv_data;
	char				buffer[1204];
	char				*msg;
};

struct					s_server
{
	int 				server_fd;
	struct sockaddr_in 	addr;
	int 				port;
	char				buffer[30000];
	long				valread;
	int					new_socket;
	int					addrlen;
	struct s_client		*client;
	std::string			url;
	std::string			method;
	std::string			protocol;
	std::vector<std::string> _reader;
	std::map<std::string, std::string> _readmap;
};

class HeaderReader: public IReader {
	public:
		HeaderReader(int fd);
		HeaderReader(const HeaderReader& rhs) {}
		HeaderReader& operator=(const HeaderReader& rhs) {
			return *this;
		}
		~HeaderReader(void) {}
		std::string _readData();
		void _sendDataBack() {}
		int	_parseData();
		int _checkMethod();
		int _checkUri();
		int _checkProtocol();
		void readConnection() {}
	public:
		int 		_fd;
		size_t		_r;
		char		*_retbuff;
		std::string _buff;
		std::string _method;
		std::string _protocol;
		std::string _uri;
		std::vector<std::string> _read;
		std::map<std::string, std::string> _rmap;
};

#endif