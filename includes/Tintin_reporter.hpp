// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Tintin_reporter.hpp                                :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/02/27 16:52:59 by ftaffore          #+#    #+#             //
//   Updated: 2015/02/27 16:53:00 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

#include <stdexcept>
#include <string>


#define				DECAL_MONTH		1
#define				DECAL_YEAR		1900

class Tintin_reporter
{

private:
	const std::string	_dirname;
	const bool			_isLock;
	int					_fd;

	Tintin_reporter(void);
	Tintin_reporter(Tintin_reporter const &);
	Tintin_reporter&	operator=(Tintin_reporter const &);

public:
	Tintin_reporter(std::string direname, bool isLock);
	~Tintin_reporter(void);

	void				writeFile(const std::string &str, const std::string type) const;

	class OpenException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "Open fail";
		}
	};

	class CdException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "Chdir fail";
		}
	};

	class WriteException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "Write fail";
		}
	};

	class LockException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "flock fail";
		}
	};
	
};

#endif