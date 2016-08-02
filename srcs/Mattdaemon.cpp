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

Mattdaemon::Mattdaemon(void) : _tintin_reporter(PATH_DIR_LOG) {
	this->_tintin_reporter.write("Matt_daemon: Started", "INFO");
	this->_tintin_reporter.write("Matt_daemon: Creating server.", "INFO");
	// umask(0777);
	// if ((this->_sid = setsid()) < 0) {		
	// 	throw Mattdaemon::SidException();
	// }
	this->_startserver();
	this->_tintin_reporter.write("Matt_daemon: Server created.", "INFO");

}

Mattdaemon::~Mattdaemon(void) {
	return ;
}

void					Mattdaemon::_startserver(void) {
	int					fdsock;
	struct sockaddr_in	sin;
	struct protoent		*pe;

	if ((pe = getprotobyname("tcp")) == NULL) {
		throw Mattdaemon::PeException();
	}

	if ((fdsock = socket(PF_INET, SOCK_STREAM, pe->p_proto)) < 0) {
		throw Mattdaemon::SocketException();
	}

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(PORT);

	if (bind(fdsock, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
		throw Mattdaemon::BindException();
	}
	if (listen(fdsock, 4242) < 0) {
		throw Mattdaemon::ListenException();
	}
	this->_fds.push_front(new Fd(FD_SERVER, fdsock));
}

bool			Mattdaemon::_isEnd(void) {
	return (true);
}

void					Mattdaemon::_accept_client(const int fdsock) {
	int					cs;
	struct sockaddr_in	csin;
	socklen_t			csin_len;

	std::cout << "accept client" << std::endl;
	csin_len = sizeof(csin);
	if ((cs = accept(fdsock, (struct sockaddr*)&csin, &csin_len)) < 0) {
		this->_tintin_reporter.write("Accept client fail.", "ERROR");
		return ;
	}
	printf("New client #%d\n", cs);
	this->_fds.push_front(new Fd(FD_CLIENT, cs));
}

void					Mattdaemon::_display_msgs(void) {
	for (std::list<std::string *>::iterator it = this->_msgs.begin(); it != this->_msgs.end(); it++) {
		this->_tintin_reporter.write(**it, "LOG");
		std::cout << **it << std::endl;
	}
    this->_msgs.clear();
}

int					Mattdaemon::_read_client(const int fd) {

	std::string 		*str = new std::string();
	char				buff[BUF_SIZE + 1];
	int					len;

	std::cout << "client read" << std::endl;
	bzero(buff, BUF_SIZE);
	while ((len = read(fd, buff, BUF_SIZE)) >= BUF_SIZE) {
		if (len <= 0) {
			return (-1);
		}
		buff[len] = '\0';
		*str += std::string(buff);
		bzero(buff, BUF_SIZE);
	}
	if (len <= 0) {
		return (-1);
	}
	buff[len] = '\0';
	*str += std::string(buff);
	this->_msgs.push_front(str);
	return (0);
}


void			Mattdaemon::_init_fd(void) {
	std::cout << "init fd" << std::endl;
	FD_ZERO(&this->_rd);
	for (std::list<Fd *>::iterator it = this->_fds.begin(); it != this->_fds.end(); it++) {
		if ((*it)->type != FD_FREE) {
			std::cout << (*it)->type << " : " << (*it)->fd << " init" << std::endl;
			FD_SET((*it)->fd, &this->_rd);
		}
	}
}


void			Mattdaemon::_loop_fd(void) {

	std::cout << "loop fd" << std::endl;
	for (std::list<Fd *>::iterator it = this->_fds.begin(); it != this->_fds.end(); it++) {
		if (FD_ISSET((*it)->fd, &this->_rd)) {
			// std::cout << (*it)->type << std::endl;
			if ((*it)->type == FD_SERVER) {
				this->_accept_client((*it)->fd);
			} else if ((*it)->type == FD_CLIENT) {
				if (this->_read_client((*it)->fd) < 0) {
					this->_fds.erase(it);
				}

			}
		}
	}
}

void		Mattdaemon::run(void) {

	this->_tintin_reporter.write("Matt_daemon: Entering Daemon mode.", "INFO");
	this->_tintin_reporter.write("Matt_daemon: started. PID: 6498.", "INFO");

	while (this->_isEnd()) {
		printf("start loop\n");
		this->_display_msgs();
		this->_init_fd();
		std::cout << "Select : " << this->_fds.size() + 3 << std::endl;
		if (select(this->_fds.size() + 3, &this->_rd, NULL, NULL, NULL) < 0) {
			throw Mattdaemon::SelectException();
		}
		this->_loop_fd();
		std::cout<< std::endl<< std::endl<< std::endl<< std::endl<< std::endl;
		usleep(500000);
	}
}
