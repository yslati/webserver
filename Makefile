# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayoub <ayoub@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/09 13:26:06 by aaqlzim           #+#    #+#              #
#    Updated: 2021/06/09 22:24:44 by ayoub            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv

SRCS := Mainer.cpp ./includes/Request.cpp
OBJS := $(SRCS:.cpp=.o)
CXX := g++
CXXFLAGS :=

all: $(NAME)
$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	@rm -rf $(OBJS)
	@echo "\033[1;32m+INFO: Objects removed!\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@echo "\033[1;32m+INFO: $(NAME) removed!\033[0m"

re: fclean all