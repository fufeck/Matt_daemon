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

#include <sys/stat.h>
#include <sys/types.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>


class Tintin_reporter
{

private:
	std::ofstream fs;

	Tintin_reporter(void);
	Tintin_reporter(Tintin_reporter const &);
	Tintin_reporter&	operator=(Tintin_reporter const &);

public:
	Tintin_reporter(std::string direname);
	~Tintin_reporter(void);

	void				write(const std::string &str, const std::string type);


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
	
};

#endif