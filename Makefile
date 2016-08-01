#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2013/11/20 15:15:50 by ftaffore          #+#    #+#              #
#    Updated: 2015/01/18 16:24:19 by chery            ###   ########.fr        #
#                                                                              #
#******************************************************************************#

CC					=	g++

RM					=	rm -rf

CFLAGS				=	-Werror -Wall -W -Wextra


NAME				=	Matt_daemon

INCLUDES			=	-I includes -I libs


LIBS				=	


SRCS				=	srcs/main.cpp				\
						srcs/Mattdaemon.cpp

OBJS				=	$(SRCS:.cpp=.o)

all					:	$(NAME) $(NAME_NCURSE)

$(NAME)				:	$(OBJS)
						$(CC) -o $@ $(CFLAGS) $(OBJS) $(LIBS) $(INCLUDES)

.cpp.o				:
						$(CC) -o $@ $(CFLAGS) $(INCLUDES) -c $<

clean				:
						$(RM) $(OBJS)

fclean				:	clean
						$(RM) $(NAME)

re					:	fclean all

.PHONY				:	all clean fclean re
