/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 18:35:32 by psalame           #+#    #+#             */
/*   Updated: 2024/05/23 16:17:00 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define NB_CLIENTS_MAX 500

#include "Client.hpp"
#include "Channel.hpp"

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

class	Client;
class	Channel;
class	Server
{
	public:
		Server();
		~Server();

		void	set_ip(std::string ip);
		void	set_port(unsigned short port);
		void	set_password(std::string password);

		std::list<Channel>	&get_channels();
		std::list<Client>	&get_clients();
		const std::string	&get_ip() const;
		
		void	create_socket();
		void	runtime();

		void	add_channel(const Channel &channel);
		void	broadcast(const std::string &request);
	
	private:
		std::string		_ip;
		unsigned short	_port;
		std::string	_password;

		std::list<Client>	_clients;
		std::list<Channel>	_channels;
		int					_sockfd;

		void	accept_client();
		void	read_client_input(Client &client);
		void	process_request(Client &client, std::string &request);
		void	process_command(Client &client, std::string &req);
		void	clean_clients();
};

#endif