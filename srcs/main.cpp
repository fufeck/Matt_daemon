// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/02/27 16:41:20 by ftaffore          #+#    #+#             //
//   Updated: 2015/02/27 16:41:20 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <iostream>
#include <signal.h>
#include "Mattdaemon.hpp"
#include "Tintin_reporter.hpp"

void					*sigleton(Mattdaemon *daemon) {
	static Mattdaemon	*ptr;
	if (daemon != NULL) {
		ptr = daemon;
		return (NULL);
	}
	return ptr;
}

void sighandler(int signum)
{
	// std::cout << "SIG" << std::endl;
	Mattdaemon 			*daemon = (Mattdaemon *)sigleton(NULL);
	(void)signum;
	daemon->finish();
}

int 				main(void)
{
	// pid_t			pid;
	Mattdaemon		*daemon;
	Tintin_reporter	*log;
	Tintin_reporter	*lock;

 	signal(SIGINT, sighandler);
 	signal(SIGABRT, sighandler);
 	signal(SIGTERM, sighandler);

 	// std::cout << "a" << std::endl;
	try {
		// CREATE LOG FILE
		log = new Tintin_reporter(PATH_DIR_LOG, false);
		log->writeFile("Matt_daemon: Started", "INFO");
	} catch (std::exception & e) {
		std::cerr << "Can't open :" << PATH_DIR_LOG << std::endl;
		delete log;
		return (-1);
	}
 	// std::cout << "b" << std::endl;
	try {
		// CREATE LOCK FILE
		lock = new Tintin_reporter(PATH_DIR_LOCK, true);
	} catch (std::exception & e) {
		std::cerr << "Can't open :" << PATH_DIR_LOCK << std::endl;
		log->writeFile("Matt_daemon: Error file locked.", "ERROR");
		log->writeFile("Matt_daemon: Quitting.", "INFO");
		delete log;
		delete lock;
		return (-1);
	}
 	// std::cout << "c" << std::endl;
	// if ((pid = fork()) < 0) {
	// 	// IF FAIL
	// 	std::cerr << "ERROR : fork fail" << std::endl;
	// 	return(-1);
	// } else if (pid > 0) {
	// 	// IF FATHER
	// 	// std::cout << "Je suis le père " << pid << std::endl;
	// 	return(0);
	// } else {
	try {
		// CREATE AND RUN DAEMON
		// std::cout << "Je suis le fils " << pid << std::endl;
		daemon = new Mattdaemon(log);
		sigleton(daemon);
		daemon->run();
	} catch (std::exception & e) {
		log->writeFile(std::string("Matt_daemon: ") + e.what(), "ERROR");
		log->writeFile("Matt_daemon: Quitting.", "INFO");
		std::cout << "END OF DAEMON CATCH" << std::endl;
		delete log;
		std::cout << "END OF DAEMON CATCH" << std::endl;
		delete lock;
		std::cout << "END OF DAEMON CATCH" << daemon << std::endl;
		delete daemon;
		std::cout << "END OF DAEMON CATCH" << std::endl;
		return (-1);
	}
	log->writeFile("Matt_daemon: Quitting.", "INFO");
	std::cout << "END OF DAEMON" << std::endl;
	delete log;
	std::cout << "END OF DAEMON" << std::endl;
	delete lock;
	std::cout << "END OF DAEMON" << std::endl;
	delete daemon;
	std::cout << "END OF DAEMON" << std::endl;
	// }
	return(0);
}