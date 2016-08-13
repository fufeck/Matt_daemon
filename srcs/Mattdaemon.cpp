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

#include <strings.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>

#include "Mattdaemon.hpp"

Mattdaemon::Mattdaemon(const Tintin_reporter *tintin_reporter) : _log(tintin_reporter) {
	// pid_t				sid;
	// std::stringstream 	ss;

	this->_isEnd = false;
	this->_log->writeFile("Matt_daemon: Creating server.", "INFO");
	this->_startserver();
	this->_log->writeFile("Matt_daemon: Server created.", "INFO");
	this->_log->writeFile("Matt_daemon: Entering Daemon mode.", "INFO");
	// if ((sid = setsid()) < 0) {
	// 	throw Mattdaemon::SidException();
	// }
	// ss << "Matt_daemon: started. PID: " << sid << ".";
	// this->_log->writeFile(ss.str(), "INFO");
}

Mattdaemon::~Mattdaemon(void) {
	
	std::cout << "STAR ENDEND" << std::endl;
	for (std::list<Fd *>::iterator it = this->_fds.begin(); it != this->_fds.end();) {
		std::cout << "LOOP CLIENT: " << it << std::endl;
		std::cout << "LOOP CLIENT: " << *it << " : " << (*it)->fd << std::endl;
		if ((*it)->type == FD_CLIENT) {
			close((*it)->fd);
			delete *it;
			this->_fds.erase(it++);
		}
	}
	for (std::list<Fd *>::iterator it = this->_fds.begin(); it != this->_fds.end();) {
		std::cout << "LOOP SERVER: " << *it << " : " << (*it)->fd << std::endl;
		if ((*it)->type == FD_SERVER) {
			close((*it)->fd);
			delete *it;
			this->_fds.erase(it++);
		}
	}
	std::cout << "ENDEND" << std::endl;
    this->_fds.clear();
	std::cout << "ENDEND" << std::endl;
    this->_msgs.clear();
	std::cout << "ENDEND" << std::endl;
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

void					Mattdaemon::_accept_client(const int fdsock) {
	int					cs;
	struct sockaddr_in	csin;
	socklen_t			csin_len;

	csin_len = sizeof(csin);
	if ((cs = accept(fdsock, (struct sockaddr*)&csin, &csin_len)) < 0) {
		this->_log->writeFile("Accept client fail.", "ERROR");
		return ;
	}
	if (this->_fds.size() < FD_MAX) {
		this->_fds.push_front(new Fd(FD_CLIENT, cs));
	} else {
		close(cs);
		this->_log->writeFile("Matt_daemon: The connection of a client was rejected because because the connection quota is reached.", "INFO");
	}


}

void					Mattdaemon::_display_msgs(void) {

	std::cout << "DISPLAY MSG" << std::endl;
	for (std::list<std::string *>::iterator it = this->_msgs.begin(); it != this->_msgs.end();) {
		std::cout << "LOOP : " << *it << " : " << **it << std::endl;
		// if (*it != NULL) {
		if ((**it).compare("quit") == 0) {
			this->_log->writeFile("Matt_daemon: Request quit.", "INFO");
			this->_isEnd = true;
		} else {
			this->_log->writeFile("Matt_daemon: User input: " + **it, "LOG");
		}
		std::cout << "LOOP : " << *it << std::endl;
		delete *it;
		std::cout << "LOOP : " << *it << std::endl;
		this->_msgs.erase(it++);
		// }
		std::cout << "LOOP : " << *it << std::endl;
	}
	// std::cout << "DISPLAY MSG END" << std::endl;
    this->_msgs.clear();
	std::cout << "DISPLAY MSG END" << std::endl;
}

int					Mattdaemon::_read_client(const int fd) {

	std::cout << "READ CLIENT : " << fd << std::endl;
	std::string 		*str = new std::string();
	char				buff[BUF_SIZE + 1];
	int					len;

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
	(*str).erase(std::remove((*str).begin(), (*str).end(), '\n'), (*str).end());
	this->_msgs.push_front(str);
	return (0);
}


void			Mattdaemon::_init_fd(void) {

	FD_ZERO(&this->_rd);
	for (std::list<Fd *>::iterator it = this->_fds.begin(); it != this->_fds.end(); it++) {
		if ((*it)->type != FD_FREE) {
			FD_SET((*it)->fd, &this->_rd);
		}
	}
}


void			Mattdaemon::_loop_fd(void) {

	for (std::list<Fd *>::iterator it = this->_fds.begin(); it != this->_fds.end();) {
		if (FD_ISSET((*it)->fd, &this->_rd)) {
			if ((*it)->type == FD_SERVER) {
				this->_accept_client((*it)->fd);
				it++;
			} else if ((*it)->type == FD_CLIENT) {
				if (this->_read_client((*it)->fd) < 0) {
					close((*it)->fd);
					delete *it;
					this->_fds.erase(it++);
					this->_log->writeFile("Matt_daemon: A client has disconnected.", "INFO");
				}
			}
		}
	}
}

void		Mattdaemon::finish(void) {

	this->_isEnd = true;
	this->_log->writeFile("Matt_daemon: Signal receive.", "INFO");
}

void					Mattdaemon::run(void) {
	
	while (!this->_isEnd) {
		this->_init_fd();
		std::cout << "BEFORE SELECT" << std::endl;
		if (select(this->_fds.size() + 5, &this->_rd, NULL, NULL, NULL) < 0) {
			if (this->_isEnd) {
				return ;
			}
			throw Mattdaemon::SelectException();
		}
		this->_loop_fd();
		this->_display_msgs();
	}
}
