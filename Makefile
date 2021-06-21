# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yslati <yslati@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/09 13:26:06 by aaqlzim           #+#    #+#              #
#    Updated: 2021/06/21 19:31:24 by yslati           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv

SRCS := Mainer.cpp ./includes/Request.cpp ./server/Client.cpp ./server/HttpServer.cpp \
		./includes/Response.cpp ./server/ServerSocket.cpp ./parsing/parsing.cpp ./server/Location.cpp \
		./server/Server.cpp
OBJS := $(SRCS:.cpp=.o)
CXX := g++
CXXFLAGS := -std=c++98

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
