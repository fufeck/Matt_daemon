// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Nibbler.hpp                                        :+:      :+:    :+:   //
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
#include <unistd.h>
#include <sys/stat.h>
#include <stdexcept>

#define						PORT		4242

class Mattdaemon
{

private:
	pid_t					_sid;
	int						_fd;
	fd_set					_buffer_rd;

	Mattdaemon(Mattdaemon const &);
	Mattdaemon&				operator=(Mattdaemon const &);

public:
	Mattdaemon(void);
	~Mattdaemon(void);

	void					run(void);


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
};

#endif