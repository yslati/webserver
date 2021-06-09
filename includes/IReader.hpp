/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IReader.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 11:49:47 by aaqlzim           #+#    #+#             */
/*   Updated: 2021/06/08 19:02:10 by ayoub            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IREADER_HPP
# define IREADER_HPP

# include <iostream>
# include <cstring>
# include <cstdlib>

// Network Lib
# include <poll.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/types.h>
// # include <sys/event.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>
# include <vector>

const int MAX = 101;

class IReader {
	public:
		virtual ~IReader() {}
		virtual	void readConnection() = 0;
		virtual int _checkMethod() = 0;
		virtual int _checkUri() = 0;
		virtual int _checkProtocol() = 0;
		virtual std::string _readData() = 0;
};

#endif