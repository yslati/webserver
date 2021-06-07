/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderReader.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 11:53:41 by aaqlzim           #+#    #+#             */
/*   Updated: 2021/06/07 16:54:45 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_READER_HPP
# define HEADER_READER_HPP

#include "IReader.hpp"

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
};

class HeaderReader: public IReader {
	public:
		HeaderReader(struct s_server *serv);
		HeaderReader(const HeaderReader& rhs);
		HeaderReader& operator=(const HeaderReader& rhs);
		~HeaderReader(void);
		void readConnection();
	public:
		struct s_server *_server;
		std::vector<struct s_server*> *_serv;
};

#endif