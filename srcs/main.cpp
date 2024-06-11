/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 18:14:32 by psalame           #+#    #+#             */
/*   Updated: 2024/05/22 10:19:50 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cstdlib>
#include <limits>
#include <csignal>
#include "utils.h"

void	empty_fct(__attribute__((unused)) int signal)
{
}

int	main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return (1);
	}
	unsigned short	port;
	if (!::ft_atoi(av[1], port))
	{
		std::cerr << "Invalid port value." << std::endl;
		return (1);
	}
	else
	{
		try {
			Server	sv;
			
			sv.set_ip("127.0.0.1");
			sv.set_port(port);
			sv.set_password(av[2]);
			
			sv.create_socket();
			std::signal(SIGINT, &empty_fct);
			std::signal(SIGQUIT, &empty_fct);
			sv.runtime();

		} catch (std::exception &err)
		{
			std::cerr << err.what() << std::endl;
			return (1);
		}
	}
	return (0);
}