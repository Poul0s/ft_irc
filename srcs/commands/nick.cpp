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

void	Nick(Client &client, Server &server, std::string &params)
{
	if (client.get_status() == SET_ID)
		std::cout << "Nick" << ":" << params << std::endl;
	if (params.empty())
	{
		client.send_request(ERR_NONICKNAMEGIVEN, "NICK :No nickname given");
		return ;
	}
	if (params.find_last_not_of(' ') != std::string::npos)
		params = params.substr(0, params.find_last_not_of(' ') + 1);
	if (params.size() > 9)
	{
		client.send_request(ERR_ERRONEUSNICKNAME, "NICK :Invalid nickname size");
		return ;
	}
	if (params == "anonymous")
	{
		client.send_request(ERR_ERRONEUSNICKNAME, "NICK :Invalid nickname");
		return ;
	}

	std::list<Client>	&clients = server.get_clients();
	std::list<Client>::iterator	clientIt = std::find(clients.begin(), clients.end(), params);
	if (clientIt != clients.end())
	{
		client.send_request(ERR_NICKNAMEINUSE, "NICK :Nickname is already in use");
		return ;
	}
	
	if (client.get_status() != SET_ID)
	{
		std::string	request = ":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + " NICK " + params;
		server.broadcast(request);
	}
	client.set_nickname(params);
}
