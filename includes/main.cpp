/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 12:14:41 by aaqlzim           #+#    #+#             */
/*   Updated: 2021/06/07 16:58:54 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeaderReader.hpp"


int main() {
	// Server-side Code
	// just Testing
	struct s_server *serv = new s_server;
	serv->client = new s_client;
	serv->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	// serv->client->msg = (char *)malloc(sizeof(char) * 23);
	serv->client->msg = strdup("HTTP/1.1 200 OK\r\n\r\n");
	// HeaderReader *header = new HeaderReader(serv);
	std::cout << "Start connection\n";
	serv->port = 8080;
	std::memset((char *)&serv->addr, 0, sizeof(serv->addr));
	serv->addr.sin_family = AF_INET;
	serv->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv->addr.sin_port = htons(serv->port);
	std::memset(serv->addr.sin_zero, '\0', sizeof(serv->addr.sin_zero));
	bind(serv->server_fd, (struct sockaddr* )&serv->addr, sizeof(serv->addr));
	
	// wait for incoming connection
	listen(serv->server_fd, 10);
	
	// set-up new socket_fd for exchanging data
	int count;
	while (true)
	{
		count = 0;
		std::cout << "\n+++++++++ Waiting for new connection on port 8080 +++++++++\n\n";
		serv->new_socket = accept(serv->server_fd, (struct sockaddr *)&serv->addr,
		(socklen_t *)&serv->addrlen);
		// Now Send and receive data
		std::memset((char *)serv->buffer, 0, sizeof(serv->buffer));
		while ((serv->valread = read(serv->new_socket, serv->buffer, 30000)) > 0)
		{
			// printf("buffer => %s\n", serv->buffer);
			serv->method = std::string(serv->buffer);
			std::cout << "buffer => " << serv->method << "\n";
			if (serv->buffer[serv->valread - 1] == '\n')
				count++;
			if (count == 2)
				continue ;
			std::memset((char *)serv->buffer, 0, sizeof(serv->buffer));
		}
		write(serv->new_socket, serv->client->msg, strlen(serv->client->msg));
		close(serv->new_socket);
	}
	return (0);
}