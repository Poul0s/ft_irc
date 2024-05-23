/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 16:42:55 by psalame           #+#    #+#             */
/*   Updated: 2024/05/23 17:57:12 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "irc_error_codes.h"
#include <string>
#include <list>
#include <algorithm>

void	Topic(Client &client, Server &server, std::string &params)
{
	if (params.empty())
	{
		client.send_request(ERR_NEEDMOREPARAMS, "TOPIC :Not enough parameters");
		return ;
	}

	std::string						channelName = params.substr(0, params.find(' '));
	std::list<Channel>				&channels = server.get_channels();
	std::list<Channel>::iterator	channel = std::find(channels.begin(), channels.end(), channelName);
	
	if (channel == channels.end())
		client.send_request(ERR_NOTONCHANNEL, "TOPIC :No such channel");
	else if (channel->get_mode(CHAN_MODE_SECRET) && !channel->is_user_in(client))
		client.send_request(ERR_NOTONCHANNEL, "TOPIC :No such channel");
	else
	{
		if (params.find(':') == std::string::npos)
		{
			if (channel->get_topic().empty())
				client.send_request(RPL_NOTOPIC, "TOPIC " + channelName + " :No topic is set");
			else
				client.send_request(RPL_TOPIC, "TOPIC " + channelName + " :" + channel->get_topic());
		}
		else if (!channel->get_mode(CHAN_MODE_TOPIC) || channel->is_user_op(client))
		{
			std::string	topic = params.substr(params.find(':') + 1);
			channel->set_topic(topic);
			std::string	request = ":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + " TOPIC " + channelName + " :" + topic;
			if (channel->get_mode(CHAN_MODE_SECRET))
				channel->broadcast(request);
			else
				server.broadcast(request);
		}
		else
		{
			std::cout << "TOPIC :You're not channel operator" << std::endl;
			client.send_request(ERR_CHANOPRIVSNEEDED, "TOPIC :You're not channel operator");
		}
	}

}