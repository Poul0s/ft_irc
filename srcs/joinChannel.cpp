/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinChannel.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:52:29 by psalame           #+#    #+#             */
/*   Updated: 2024/05/22 10:17:30 by psalame          ###   ########.fr       */
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
	std::string channelsPasswordStr;
	if (params.find(' ') != std::string::npos)
		channelsPasswordStr = params.find_first_not_of(' ', params.find(' '));

	while (!channelsNameStr.empty())
	{
		channelData_t	ChannelData;
		ChannelData.first = channelsNameStr.substr(0, channelsNameStr.find(','));
		if (channelsNameStr.find(','))
			channelsNameStr.substr(channelsNameStr.find(',') + 1);
		ChannelData.second = channelsPasswordStr.substr(0, channelsPasswordStr.find(','));
		if (channelsPasswordStr.find(','))
			channelsPasswordStr.substr(channelsPasswordStr.find(',') + 1);

		ChannelsData.push_back(ChannelData);
	}

	for (std::list<channelData_t>::iterator it = ChannelsData.begin(); it != ChannelsData.end(); it++)
	{
		if (it->first == "0")
		{
			// leave channel
		}
		else
		{
			std::list<Channel>					&channels = server.get_channels();
			std::list<Channel>::iterator	channel = std::find(channels.begin(), channels.end(), it->first);
			if (channel != channels.end())
			{

				if (channel->get_password() != it->second)
					client.send_request(ERR_BADCHANNELKEY, "#" + it->first + " :Cannot join channel (+k) - bad key");
				// else if () // test limit
				// 	;
				// else if () // test ban
				// 	;
				else
				{
					channel->add_user(client);
				}
			}
			else
			{
				// not found, what happen now ? (todo)
			}
		}
	}
}
