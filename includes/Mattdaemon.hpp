// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Mattdaemon.hpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/02/27 16:52:59 by ftaffore          #+#    #+#             //
//   Updated: 2015/02/27 16:53:00 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef MATTDAEMON_HPP
# define MATTDAEMON_HPP

#include <iostream>
#include <stdexcept>
#include <list>
#include <string>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <netinet/in.h>
#include <time.h>

#include "Fd.hpp"
#include "Tintin_reporter.hpp"

#define						FD_MAX			4
#define						PORT			4242
#define						BUF_SIZE		256
#define						PATH_DIR_LOG	"/tmp/matt_daemon/matt_daemon.log"
// #define						PATH_DIR_LOG	"/var/log/matt_daemon/matt_daemon.log"

enum TypeFd {FD_FREE, FD_CLIENT, FD_SERVER};

class Mattdaemon
{

private:
	Tintin_reporter				_tintin_reporter;

	pid_t						_sid;

	fd_set						_rd;
	std::list<Fd *>				_fds;
	std::list<std::string *>	_msgs;

	bool						_isEnd(void);
	void						_accept_client(const int fdsock);
	int							_read_client(const int fd);
	void						_init_fd(void);
	void						_loop_fd(void);
	void						_startserver(void);
	void						_display_msgs(void);

	Mattdaemon(Mattdaemon const &);
	Mattdaemon&					operator=(Mattdaemon const &);

public:
	Mattdaemon(void);
	~Mattdaemon(void);

	void						run(void);


	class ForkException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "Fork fail";
		}
	};

	class SidException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "Get id son fail";
		}
	};

	class PeException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "Getprotobyname fail";
		}
	};

	class SocketException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "Socket fail";
		}
	};

	class BindException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "Bind fail";
		}
	};

	class ListenException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "Listen fail";
		}
	};

	class SelectException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "Select fail";
		}
	};

	class AcceptException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "Accept fail";
		}
	};
};

#endif