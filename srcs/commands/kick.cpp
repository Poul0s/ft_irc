/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 07:52:41 by psalame           #+#    #+#             */
/*   Updated: 2024/05/24 11:31:45 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "irc_error_codes.h"
#include "utils.h"
#include <algorithm>

static void	KickUsersFromChannel(Client &author, Server &server, Channel &channel, std::list<std::string> &kickedUserList, std::string &reason)
{
	for (std::list<std::string>::iterator it = kickedUserList.begin(); it != kickedUserList.end(); it++)
	{
		std::list<Client>	&clients = server.get_clients();
		std::list<Client>::iterator	client = std::find(clients.begin(), clients.end(), *it);
		if (client == clients.end())
		{
			author.send_request(ERR_NOSUCHNICK, "KICK" + *it + " :No such nick/channel");
			return ;
		}
		else if (!channel.is_user_in(*client))
		{
			author.send_request(ERR_USERNOTINCHANNEL, "KICK" + *it + " :User not in channel");
			return ;
		}
		else
		{
			std::string	request = ":" + client->get_nickname() + "!" + client->get_username() + "@" + client->get_ip() + " KICK " + channel.get_name() + " " + client->get_nickname();
			if (!reason.empty())
				request += " :" + reason;
			channel.broadcast(request);
			channel.remove_user(*client);
		}
	}
}

void	Kick(Client &client, Server &server, std::string &params)
{
	if (params.find(' ') == std::string::npos || params.find_first_not_of(' ', params.find(' ')) == std::string::npos)
	{
		std::cout << "params: " << params << std::endl;
		client.send_request(ERR_NEEDMOREPARAMS, "KICK :Not enough parameters");
		return ;
	}
	std::string		channelsName = params.substr(0, params.find(' '));
	std::string		kickedUser = params.substr(params.find_first_not_of(' ', params.find(' ')));
	kickedUser = kickedUser.substr(0, kickedUser.find(' '));
	if (channelsName.empty() || kickedUser.empty())
	{
		client.send_request(ERR_NEEDMOREPARAMS, "KICK :Not enough parameters");
		return ;
	}
	if (channelsName.find(':') != std::string::npos || kickedUser.find(':') != std::string::npos)
	{
		client.send_request(ERR_NEEDMOREPARAMS, "KICK :Not enough parameters");
		return ;
	}
	std::string		reason = params.substr(std:: min(params.find(':'), params.size() - 1) + 1);

	std::list<Channel>		&channels = server.get_channels();
	std::list<std::string>	channelsNameList = ft_split(channelsName, ',');
	std::list<std::string>	kickedUserList = ft_split(kickedUser, ',');
	for (std::list<std::string>::iterator it = channelsNameList.begin(); it != channelsNameList.end(); it++)
	{
		std::list<Channel>::iterator	channel = std::find(channels.begin(), channels.end(), *it);
		if (channel == channels.end())
			client.send_request(ERR_NOSUCHCHANNEL, "KICK " + channel->get_name() + " :No such channel");
		else if (channel->get_mode(CHAN_MODE_SECRET) && !channel->is_user_in(client))
			client.send_request(ERR_CANNOTSENDTOCHAN, "KICK " + channel->get_name() + " :Cannot send to channel");
		else if (!channel->is_user_op(client))
			client.send_request(ERR_CHANOPRIVSNEEDED, "KICK " + channel->get_name() + " :You're not channel operator");
		else
			KickUsersFromChannel(client, server, *channel, kickedUserList, reason);
	}
}