/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinChannel.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:52:29 by psalame           #+#    #+#             */
/*   Updated: 2024/05/23 17:12:22 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <algorithm>
#include "irc_error_codes.h"

typedef std::pair<std::string, std::string> channelData_t;

void	JoinChannel(Client &client, Server &server, std::string &params)
{
	std::list<channelData_t> ChannelsData; // first: channelName, second: channelPassword
	std::string	channelsNameStr = params.substr(0, params.find(' '));
	if (channelsNameStr.empty())
	{
		client.send_request(ERR_NEEDMOREPARAMS, "JOIN :Not enough parameters");
		return ;
	}
	std::string channelsPasswordStr;
	if (params.find(' ') != std::string::npos)
		channelsPasswordStr = params.find_first_not_of(' ', params.find(' '));

	while (!channelsNameStr.empty())
	{
		channelData_t	ChannelData;
		ChannelData.first = channelsNameStr.substr(0, channelsNameStr.find(','));
		if (channelsNameStr.find(',') != std::string::npos)
			channelsNameStr.substr(channelsNameStr.find(',') + 1);
		else
			channelsNameStr = "";
		ChannelData.second = channelsPasswordStr.substr(0, channelsPasswordStr.find(','));
		if (channelsPasswordStr.find(',') != std::string::npos)
			channelsPasswordStr.substr(channelsPasswordStr.find(',') + 1);
		else
			channelsPasswordStr = "";

		ChannelsData.push_back(ChannelData);
	}

	for (std::list<channelData_t>::iterator it = ChannelsData.begin(); it != ChannelsData.end(); it++)
	{
		if (it->first == "0")
		{
			// todo leave channels
		}
		else
		{
			std::list<Channel>					&channels = server.get_channels();
			std::list<Channel>::iterator	channel = std::find(channels.begin(), channels.end(), it->first);
			if (channel != channels.end())
			{
				if (channel->get_password() != it->second)
					client.send_request(ERR_BADCHANNELKEY, it->first + " :Cannot join channel (+k) - bad key");
				else if (channel->is_full())
					client.send_request(ERR_CHANNELISFULL, it->first + " :Cannot join channel (+l) - channel is full");
				else if (channel->is_user_banned(client)) // test ban
					client.send_request(ERR_BANNEDFROMCHAN, it->first + " :Cannot join channel (+b) - banned from channel");
				else
					channel->add_user(client);
			}
			else
			{
				if (client.is_op() && it->first[0] == '#')
				{
					Channel channel(it->first, it->second);
					channel.toggle_mode(CHAN_MODE_TOPIC, true);
					channel.toggle_mode(CHAN_MODE_NO_EXTERNAL_MSG, true);
					
					server.add_channel(channel);
					server.get_channels().back().add_user(client); // todo set user op on channel
					server.get_channels().back().set_user_op(client, true);
				}
				else
					client.send_request(ERR_NOSUCHCHANNEL, it->first + " :No such channel");
			}
		}
	}
}