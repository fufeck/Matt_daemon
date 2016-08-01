// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Nibbler.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/02/27 17:46:59 by ftaffore          #+#    #+#             //
//   Updated: 2015/02/27 17:47:00 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Mattdaemon.hpp"

Mattdaemon::Mattdaemon(void) {
	umask(0);
	if ((this->sid = setsid()) < 0) {		
		throw Mattdaemon::SidException();
	}
	this->_startserver();
	std::cout << "Je suis le fils " << sid << std::endl;
}

Mattdaemon::~Mattdaemon(void) {

	return ;
}

void			Mattdaemon::_startserver(void) {

	struct sockaddr_in	sin;
	struct protoent		*pe;

	if ((pe = getprotobyname("tcp")) == NULL) {
		throw Mattdaemon::PeException();
	}

	if ((this->fd = socket(PF_INET, SOCK_STREAM, pe->p_proto)) < 0) {
		throw Mattdaemon::SocketException();
	}

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(PORT);

	if (bind(this->fd, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
		throw Mattdaemon::BindException();
	}
	if (listen(this->fd, 42) < 0) {
		throw Mattdaemon::ListenException();
	}
	return (0);
}







void			Mattdaemon::_startserver(void) {
     int sockfd, newsockfd, portno;

     socklen_t clilen;
     char buffer[256];

     struct sockaddr_in serv_addr, cli_addr;

     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
 
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
		error("ERROR on accept");
     bzero(buffer,256);

     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);
     return 0; 
}



bool			Mattdaemon::_isEnd(void) {
	return (false);
}

static void		init_fd(t_serv *s)
{
	int			i;

	i = 0;
	s->maxfd = 0;
	FD_ZERO(&s->rd);
	FD_ZERO(&s->wr);
	while (i < FD_MAX)
	{
		if (s->fds[i].type != FD_FREE)
		{
			if (s->fds[i].cmd == NULL && s->fds[i].type != FD_GFX)
				FD_SET(i, &s->rd);
			if (s->fds[i].msg != NULL)
				FD_SET(i, &s->wr);
			s->maxfd = i;
		}
		i++;
	}
}

static int		check_fd(t_serv *s, int i)
{
	int			ret;

	while (i < FD_MAX)
	{
		if (s->fds[i].type != FD_FREE)
		{
			ret = 0;
			if (FD_ISSET(i, &s->rd))
				ret = s->fds[i].fct_read(s, i);
			else if (FD_ISSET(i, &s->wr))
				ret = s->fds[i].fct_write(s, i);
			if (ret == 0 && s->fds[i].type == FD_PLAYER)
				ret = player_turn(s, i);
			if (ret < 0)
				client_quit(s, i);
		}
		i++;
	}
	return (0);
}

void		Mattdaemon::run(void) {
	t_time		tv;
	tv.tv_sec = s->t.tv_sec;
	tv.tv_usec = s->t.tv_usec;

	printf("Zappy starting...\n");
	while (this->_isEnd())
	{
		// init_fd(s);
		if (select(2, this->rd, NULL, NULL, &tv) < 0) {
			throw Mattdaemon::SelectException();
		}
		// check_fd(s, 0);
		// baby_turn(s);
		// check_life(s);
	}
}
