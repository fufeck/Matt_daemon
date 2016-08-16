// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Tintin_reporter.cpp                                :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/02/27 17:46:59 by ftaffore          #+#    #+#             //
//   Updated: 2015/02/27 17:47:00 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //


#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "Tintin_reporter.hpp"

Tintin_reporter::Tintin_reporter(std::string dirname, bool isLock) :  _dirname(dirname), _isLock(isLock), _fd(0) {
	std::string 	delimiter = "/";
	size_t 			pos = 0;
	std::string 	token;

	while ((pos = dirname.find(delimiter)) != std::string::npos) {
	    token = dirname.substr(0, pos);
	    if (chdir(std::string(token + "/").c_str()) < 0) {
			if (mkdir(token.c_str(), 0777) < 0) {
				throw Tintin_reporter::CdException();
			}
			if (chdir(std::string(token + "/").c_str()) < 0) {
				throw Tintin_reporter::CdException();
			}
	    }
	    dirname.erase(0, pos + delimiter.length());
	}
	if (isLock) {
		if (access(dirname.c_str(), F_OK) < 0) {
			if ((this->_fd = open(dirname.c_str(), O_CREAT)) < 0) {
				
				throw Tintin_reporter::OpenException();
			}
		} else  {
			throw Tintin_reporter::OpenException();
		}
	} else {
		if ((this->_fd = open(dirname.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) {
			throw Tintin_reporter::OpenException();
		}
	}
}


Tintin_reporter::~Tintin_reporter(void) {
	if (this->_fd > 0) {
		close(this->_fd);
		if (this->_isLock) {
			remove(this->_dirname.c_str());
		}
	}
}

void				Tintin_reporter::writeFile(const std::string &str, const std::string type) const {
	time_t 			t = time(NULL);
	struct 			tm *ti = localtime(&t);
	std::stringstream 	ss;

	lseek(this->_fd, 0, SEEK_END);
	ss << "[" << std::setw(2) << std::setfill('0') << ti->tm_mday << "/" << std::setw(2) << std::setfill('0') << (ti->tm_mon + DECAL_MONTH) << "/" << (ti->tm_year + DECAL_YEAR);
	ss << "-" << std::setw(2) << std::setfill('0') << ti->tm_hour << ":" << std::setw(2) << std::setfill('0') << ti->tm_min << ":" << std::setw(2) << std::setfill('0') << ti->tm_sec << "] ";
	ss << "[ " << type << " ] - ";
	ss << str << std::endl;
	if (write(this->_fd, ss.str().c_str(), ss.str().size()) < 0) {
		throw Tintin_reporter::WriteException();
	}
	return ;
}