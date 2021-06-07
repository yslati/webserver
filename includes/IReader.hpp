/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IReader.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 11:49:47 by aaqlzim           #+#    #+#             */
/*   Updated: 2021/06/07 11:52:34 by aaqlzim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IREADER_HPP
# define IREADER_HPP

# include <iostream>
# include <unistd.h>

class IReader {
	public:
		virtual ~IReader() {}
		virtual	void readConnection() const = 0;
};

#endif