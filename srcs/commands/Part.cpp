/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:30:28 by psalame           #+#    #+#             */
/*   Updated: 2024/05/24 15:39:01 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "irc_error_codes.h"
#include "utils.h"
#include <string>
#include <algorithm>

void	Part(Client &client, Server &server, std::string &params)
{
	params = params.substr(0, std::min(params.find(' '), params.size()));
	std::list<std::string>	arguments = ft_split(params, ',');
	if (arguments.size() == 0)
	{
		client.send_request(ERR_NEEDMOREPARAMS, "PART :Not enough parameters");
		return ;
	}

	std::list<Channel>	&channels = server.get_channels();
	for (std::list<std::string>::iterator it = arguments.begin(); it != arguments.end(); it++)
	{
		std::list<Channel>::iterator	channel = std::find(channels.begin(), channels.end(), *it);
		if (channel == channels.end())
			client.send_request(ERR_NOSUCHCHANNEL, "PART " + *it + " :No such channel");
		else if (!channel->is_user_in(client))
			client.send_request(ERR_NOTONCHANNEL, "PART " + *it + " :You're not on that channel");
		else
		{
			std::string	request = ":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + " PART " + *it;
			channel->broadcast(request);
			channel->remove_user(client);
		}
	}
}