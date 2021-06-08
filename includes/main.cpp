/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 12:14:41 by aaqlzim           #+#    #+#             */
/*   Updated: 2021/06/08 17:23:09 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeaderReader.hpp"


// int isSocket(int socket_fd, int fd) {
// 	if (socket_fd == fd)
// 		return fd;
// 	return socket_fd;
// }

// int main() {
// 	// Server-side Code
// 	// just Testing
// 	fd_set master_set;
// 	fd_set worker_set;
// 	fd_set response_set;
// 	struct s_server *serv = new s_server;
// 	serv->client = new s_client;
// 	serv->server_fd = socket(AF_INET, SOCK_STREAM, 0);
// 	// serv->client->msg = (char *)malloc(sizeof(char) * 23);
// 	serv->client->msg = strdup("HTTP/1.1 200 OK\r\n\r\n");
// 	// HeaderReader *header = new HeaderReader(serv);
// 	std::cout << "Start connection\n";
// 	serv->port = 8080;
// 	std::memset((char *)&serv->addr, 0, sizeof(serv->addr));
// 	serv->addr.sin_family = AF_INET;
// 	serv->addr.sin_addr.s_addr = htonl(INADDR_ANY);
// 	serv->addr.sin_port = htons(serv->port);
// 	std::memset(serv->addr.sin_zero, '\0', sizeof(serv->addr.sin_zero));
// 	bind(serv->server_fd, (struct sockaddr* )&serv->addr, sizeof(serv->addr));
	
// 	// wait for incoming connection
// 	listen(serv->server_fd, 10);
	
// 	// set-up new socket_fd for exchanging data
// 	int count;
// 	FD_ZERO(&master_set);
// 	std::map<int, std::string> rmap;
// 	while (true)
// 	{
// 		worker_set = master_set;
// 		std::cout << "\n+++++++++ Waiting for new connection on port 8080 +++++++++\n\n";
// 		if (select(FD_SETSIZE, &worker_set, NULL, NULL, NULL) < 0)
// 		{
// 			std::cerr << "Select Failed\n";
// 			continue;
// 		}
// 		for (int i = 0; i < FD_SETSIZE; i++) {
// 			if (FD_ISSET(i, &worker_set)) {
// 				// int fd;
// 				// if ((fd = isSocket(serv->server_fd, i)) != FD_SETSIZE - 1)
// 				// {
// 				// 	serv->new_socket = accept(serv->server_fd, (struct sockaddr *)&serv->addr,
// 				// 	(socklen_t *)&serv->addrlen);
// 				// 	FD_SET(serv->new_socket, &master_set);
// 				// } else {
// 					std::cout << "Connection should we read from" << std::endl;
// 					std::string str;
// 					str = "HTTP/1.1 200 OK\r\n";
// 					str += "Content-Type: text/html\r\n";
// 					str += "Content-Length: 11\r\n";
// 					str += "\r\n";
// 					str += "Hello World";
// 					std::pair<int, std::string> p(i, str);
// 					rmap.insert(p);
// 					FD_SET(i, &response_set);
// 				// }
// 			}
// 		}
// 		for (int i = 0; i < FD_SETSIZE; i++) {
// 			if (FD_ISSET(i, &response_set)) {
// 				std::map<int, std::string>::iterator it = rmap.find(i);
// 				if (it != rmap.end())
// 					send(i, it->second.c_str(), it->second.size(), 0);
// 				FD_CLR(i, &response_set);
// 			}
// 		}
// 		// Now Send and receive data
// 		// std::memset((char *)serv->buffer, 0, sizeof(serv->buffer));
// 		// while ((serv->valread = recv(serv->new_socket, serv->buffer, 30000, 0)) > 0)
// 		// {
// 		// 	printf("buffer => %s\n", serv->buffer);
			
// 		// 	if (serv->buffer[serv->valread - 1] == '\n')
// 		// 		break ;
// 		// 	std::memset((char *)serv->buffer, 0, sizeof(serv->buffer));
// 		// }
// 		// write(serv->new_socket, serv->client->msg, strlen(serv->client->msg));
// 		// close(serv->new_socket);
// 	}
// 	return (0);
// }

int main()
{
	char buff[1024];
	char endline[1];
	std::memset((char *)buff, 0, 1024);
	std::memset((char *)endline, 0, 1);
	int r;
	int l;

	while ((r = read(1, buff, 1024)) > 0)
	{
		buff[r] = '\0';
		if (buff[r - 1] == '\n')
		{
			HeaderReader h(1);
			h._parseData();
			h._checkUri();
			if (h._checkMethod())
				std::cout << h._method << "\n";
			// Excute code
		}
	}
	return (0);
}