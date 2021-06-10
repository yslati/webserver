# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/09 13:26:06 by aaqlzim           #+#    #+#              #
#    Updated: 2021/06/10 16:05:57 by aaqlzim          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv

SRCS := ./server/test.cpp ./includes/Request.cpp \
		./includes/Response.cpp ./server/ServerSocket.cpp
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