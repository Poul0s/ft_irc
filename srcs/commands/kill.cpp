/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycontre <ycontre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:29:33 by psalame           #+#    #+#             */
/*   Updated: 2024/05/30 19:02:07 by ycontre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "irc_error_codes.h"
#include <string>
#include "utils.h"
#include <algorithm>

void	Kill(Client &client, Server &server, std::string &params)
{
	if (!client.is_op())
	{
		client.send_request(ERR_NOPRIVILEGES, "KILL :No such permissions.");
		return ;
	}
	if (params.empty())
	{
		client.send_request(ERR_NEEDMOREPARAMS, "KILL :Need more params.");
		return ;
	}

	std::string	usersParam = params.substr(0, std::min(params.find(' '), params.size()));
	std::string	reason = "No reason given";
	if (params.find(' ') != std::string::npos && params.find_first_not_of(' ', params.find(' ')) != std::string::npos)
		reason = params.substr(params.find_first_not_of(' ', params.find(' ')));
	std::vector<std::string> users = ft_split(usersParam, ' ');
	for (std::vector<std::string>::iterator it = users.begin(); it != users.end(); it++)
	{
		std::list<Client>	&clients = server.get_clients();
		std::list<Client>::iterator	client = std::find(clients.begin(), clients.end(), *it);
		if (client != clients.end())
			client->disconnect(reason);
	}
}
