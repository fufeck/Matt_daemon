// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Fd.hpp                                             :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/02/27 16:52:59 by ftaffore          #+#    #+#             //
//   Updated: 2015/02/27 16:53:00 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef FD_HPP
# define FD_HPP

#include <unistd.h>
#include <sys/stat.h>
#include <stdexcept>

class Fd
{

private:
	Fd(void);
	Fd(Fd const &);
	Fd&				operator=(Fd const &);

public:
	Fd(int type, int fd);
	~Fd(void);

	const int				type;
	const int				fd;
};

#endif