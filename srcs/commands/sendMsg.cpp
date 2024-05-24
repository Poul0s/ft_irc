/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 14:06:39 by psalame           #+#    #+#             */
/*   Updated: 2024/05/24 07:33:38 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "irc_error_codes.h"
#include <algorithm>

void	SendChannelMsg(Client &client, Server &server, std::string &channelName, std::string &message)
{
	std::list<Channel>				&channels = server.get_channels();
	std::list<Channel>::iterator	channel = std::find(channels.begin(), channels.end(), channelName);

	if (channel == channels.end())
		client.send_request(ERR_CANNOTSENDTOCHAN, "PRIVMSG :Cannot send to channel");
	else
	{
		if (channel->get_mode(CHAN_MODE_NO_EXTERNAL_MSG) && !channel->is_user_in(client))
			client.send_request(ERR_CANNOTSENDTOCHAN, "PRIVMSG :Cannot send to channel");
		else if (channel->is_user_banned(client))
			client.send_request(ERR_CANNOTSENDTOCHAN, "PRIVMSG :Cannot send to channel");
		else
		{
			std::string	request = ":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + " PRIVMSG " + channelName + " :" + message;
			channel->broadcast(request, client);
		}
	}
}

void	SendUserMsg(Client &client, Server &server, std::string &userName, std::string &message)
{
	std::list<Client>	&clients = server.get_clients();
	std::list<Client>::iterator	clientIt = std::find(clients.begin(), clients.end(), userName);

	if (clientIt == clients.end())
		client.send_request(ERR_NOSUCHNICK, "PRIVMSG :No such nick");
	else
	{
		std::string	request = ":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + " PRIVMSG " + userName + " :" + message;
		clientIt->send_request(request);
	}
}

void	SendMsg(Client &client, Server &server, std::string &params)
{
	if (params.find(' ') == std::string::npos)
	{
		client.send_request(ERR_NORECIPIENT, "PRIVMSG :No Recipient given");
		return ;
	}
	if (params.find(':', params.find(' ')) == std::string::npos)
	{
		client.send_request(ERR_NOTEXTTOSEND, "PRIVMSG :No text to send");
		return ;
	}
	std::string	destinations = params.substr(0, params.find(' '));
	std::string	message = params.substr(params.find(':', params.find(' ')) + 1);

	while (!destinations.empty())
	{
		std::string	destination = destinations.substr(0, destinations.find(','));
		destinations = destinations.substr(std::min(destinations.find(','), destinations.size()));
		if (destination[0] == '#' || destination[0] == '&')
			SendChannelMsg(client, server, destination, message);
		else
			SendUserMsg(client, server, destination, message);
	}
}