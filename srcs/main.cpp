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

#include "Mattdaemon.hpp"

int main(void)
{
	Mattdaemon	*daemon;
	pid_t				pid;
 
	if ((pid = fork()) < 0) {
		// IF FAIL
		std::cerr << "ERROR : fork fail" << std::endl;
		return( EXIT_FAILURE );
	} else if (pid > 0) {
		// IF FATHER
		std::cout << "Je suis le père " << pid << std::endl;
		return( EXIT_SUCCESS );
	} else {
		try {
			daemon = new Mattdaemon();
			daemon->run();
		} catch (std::exception & e) {
			std::cerr << "Exception : " << e.what() << std::endl;
		}
	}
	return( EXIT_SUCCESS );
}