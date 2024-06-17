/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:22:44 by psalame           #+#    #+#             */
/*   Updated: 2024/05/24 14:46:05 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "irc_error_codes.h"
#include <iostream>
#include <algorithm>
#include "utils.h"

void	Nick(Client &client, Server &server, std::string &params)
{
	if (client.get_status() == SET_ID)
		std::cout << "Nick" << ":" << params << std::endl;
	
	std::vector<std::string> args = ft_split(params, ' ');
	
	if (args.size() == 0)
	{
		client.send_request(ERR_NONICKNAMEGIVEN, "NICK :No nickname given");
		return ;
	}
	if (args.size() != 1)
	{
		client.send_request(ERR_ERRONEUSNICKNAME, "NICK :Nickname cannot have space.");
		return ;
	}
	if (args[0].size() > 9)
	{
		client.send_request(ERR_ERRONEUSNICKNAME, "NICK :Invalid nickname size");
		return ;
	}
	if (args[0] == "anonymous")
	{
		client.send_request(ERR_ERRONEUSNICKNAME, "NICK :Invalid nickname");
		return ;
	}

	std::list<Client>	&clients = server.get_clients();
	std::list<Client>::iterator	clientIt = std::find(clients.begin(), clients.end(), args[0]);
	if (clientIt != clients.end())
	{
		client.send_request(ERR_NICKNAMEINUSE, "NICK :Nickname is already in use");
		if (client.get_failNickname().empty())
			client.set_failNickname(args[0]);
		return ;
	}
	
	if (client.get_status() != SET_ID)
	{
		std::string	request = ":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + " NICK " + args[0];
		server.broadcast(request);
	}
	else if (!client.get_failNickname().empty())
	{
		std::string	request = ":" + client.get_failNickname() + "!" + client.get_username() + "@" + client.get_ip() + " NICK " + args[0];
		client.send_request(request);
	}
	client.set_nickname(args[0]);
}
