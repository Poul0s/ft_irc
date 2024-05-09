/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 18:35:32 by psalame           #+#    #+#             */
/*   Updated: 2024/05/09 19:25:40 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define NB_CLIENTS_MAX 500

#include "Client.hpp"
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <stdbool.h>
#include <asm-generic/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <list>
#include <iostream>
#include <poll.h>

class Server
{
	public:
		Server();
		~Server();

		void	set_ip(std::string ip);
		void	set_port(int port);
		void	set_password(std::string password);
		
		void	create_socket();
		void	runtime();
		
		void	accept_client();
	
	private:
		std::string	_ip;
		int			_port;
		std::string	_password;

		std::list<Client>	_clients;
		int					_sockfd;
};

#endif