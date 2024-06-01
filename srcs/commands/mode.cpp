/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:13:38 by ycontre           #+#    #+#             */
/*   Updated: 2024/06/01 15:13:09 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "Server.hpp"
#include "utils.h"
#include "irc_error_codes.h"

#include <list>
#include <cmath>

int		parse_mode(char c)
{
	std::string modes = "opsitmnl";
	std::size_t		pos;

	pos = modes.find(c);
	if (pos == std::string::npos)
		return (-1);
	return (std::pow(pos, 2));
}

void	Mode(Client &client, Server &server, std::string &params)
{
	int		mode;
	bool	toggle;

	if (params.empty())
		{client.send_request(ERR_NEEDMOREPARAMS, "MODE :Need more params."); return;}
	
	std::vector<std::string> args = ft_split(params, ' ');

	std::list<Channel>				&channels = server.get_channels();
	std::list<Channel>::iterator	channel = std::find(channels.begin(), channels.end(), args[0]);
	
	if (channel == channels.end())
		{client.send_request(ERR_NOSUCHCHANNEL, "MODE :No such channels."); return;}

	if (args.size() == 1)
		return ; //todo send user channels mode

	if (args[1][0] != '+' && args[1][0] != '-')
		{client.send_request(ERR_NEEDMOREPARAMS, "MODE :Missing toggle token."); return;}

	toggle = (args[1][0] == '+');

	for (size_t i = 1; i < args[1].size(); i++)
	{
		mode = parse_mode(args[1][i]);
		if (mode == -1)
			{client.send_request(ERR_UNKNOWNMODE, "MODE " + args[1].substr(i, 1) + " :Unknown mode."); return;}
		if (mode == CHAN_OPER)
			return ; //todo
		else if (mode == CHAN_MAX_USERS)
			return ; //todo
		channel->toggle_mode((t_channel_mode) mode, toggle);
	}
	// todo send users in server new channel mode (or only in channel if channel secret mode is active)
}