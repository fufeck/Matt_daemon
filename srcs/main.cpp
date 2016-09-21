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
	Mattdaemon 			*daemon = (Mattdaemon *)sigleton(NULL);
	(void)signum;
	daemon->finish();
}

int 				main(void)
{
	pid_t			pid;

 	signal(SIGINT, sighandler);
 	signal(SIGABRT, sighandler);
 	signal(SIGTERM, sighandler);

	if ((pid = fork()) < 0) {
		std::cerr << "ERROR : fork fail" << std::endl;
		return(-1);
	} else if (pid > 0) {
		return(0);
	} else {
		try {
			Tintin_reporter		log(PATH_DIR_LOG, false);
			log.writeFile("Matt_daemon: Started", "INFO");
			try {

				Tintin_reporter		lock(PATH_DIR_LOCK, true);

				try {
					Mattdaemon		daemon(&log);
					
					sigleton(&daemon);
					daemon.run();
					log.writeFile("Matt_daemon: Quitting.", "INFO");
					return (0);
				} catch (std::exception & e) {
					log.writeFile(std::string("Matt_daemon: ") + e.what(), "ERROR");
					log.writeFile("Matt_daemon: Quitting.", "INFO");
					return (-1);
				}
				log.writeFile("Matt_daemon: Quitting.", "INFO");
			} catch (std::exception & e) {
				std::cerr << "ERROR : " << PATH_DIR_LOCK << " is lock." << std::endl;
				log.writeFile("Matt_daemon: Error file locked.", "ERROR");
				log.writeFile("Matt_daemon: Quitting.", "INFO");
				return (-1);
			}
		} catch (std::exception & e) {
			std::cerr << "Can't open :" << PATH_DIR_LOG << std::endl;
			return (-1);
		}
	}
	return(0);
}