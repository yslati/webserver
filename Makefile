# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mymik <mymik@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/09 13:26:06 by aaqlzim           #+#    #+#              #
#    Updated: 2021/06/25 11:23:27 by mymik            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := webserv

SRCS := Mainer.cpp ./includes/Request.cpp ./server/Client.cpp ./server/HttpServer.cpp \
		./includes/Response.cpp ./server/ServerSocket.cpp ./parsing/parsing.cpp ./server/Location.cpp \
		./server/Server.cpp ./server/UnchunkContent.cpp ./server/HexConvertor.cpp ./server/RequestValidator.cpp
OBJS := $(SRCS:.cpp=.o)
CXX := clang++
CXXFLAGS := -std=c++98 -Wall -Wextra -Werror

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
