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

#include <stdlib.h>
#include <unistd.h>
#include "Tintin_reporter.hpp"

Tintin_reporter::Tintin_reporter(std::string dirname) {
	(void)dirname;
	// std::string delimiter = "/";
	// size_t pos = 0;
	// std::string token;

	// while ((pos = dirname.find(delimiter)) != std::string::npos) {
	//     token = dirname.substr(0, pos);
	//     // std::cout << std::string(token + "/").c_str() << std::endl;
	//     if (chdir(std::string(token + "/").c_str()) < 0) {
	// 		if (mkdir(token.c_str(), 0777) < 0) {
	// 			throw Tintin_reporter::CdException();
	// 		}
	//     }
	//     dirname.erase(0, pos + delimiter.length());
	// }
	// try {
	// 	this->fs.open(dirname, std::fstream::trunc);
	// 	// std::cout << "file " << dirname << " is open" << std::endl;
	// } catch (std::fstream::failure e) {
	// 	throw Tintin_reporter::OpenException();
 //  	}
	// return ;
}

Tintin_reporter::~Tintin_reporter(void) {
 //  	this->fs.close();
	// return ;
}

void				Tintin_reporter::write(const std::string &str, const std::string type) {
	(void)str;
	(void)type;
	// time_t t = time(NULL);
	// struct tm *ti = localtime(&t);

	// // std::cout << "[" << ti->tm_mday << "/" << ti->tm_mon << "/" << ti->tm_year << "-" << ti->tm_hour << "-" << ti->tm_min << "-" << ti->tm_sec << "]";
	// // std::cout << "[ " << type << " ] - ";
	// // std::cout <<  str << std::endl;

	// this->fs << "[" << ti->tm_mday << "/" << ti->tm_mon << "/" << ti->tm_year << "-" << ti->tm_hour << "-" << ti->tm_min << "-" << ti->tm_sec << "]";
	// this->fs << "[ " << type << " ] - ";
	// this->fs << str << std::endl;
	// return ;
}