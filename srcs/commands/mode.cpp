/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:13:38 by ycontre           #+#    #+#             */
/*   Updated: 2024/06/11 16:46:14 by psalame          ###   ########.fr       */
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

static void	oper_chan_user(Client &author, Channel &channel, bool toggle, std::vector<std::string> &args)
{
	std::size_t	i;

	i = args[1].find('l') == std::string::npos ? 2 : 3;
	if (args.size() <= i)
		author.send_request(ERR_NEEDMOREPARAMS, "MODE :Missing user param for mode 'o'.");
	throw std::runtime_error("TODO");
	if (channel.is_user_in(args[i]))
		author.send_request(ERR_NOTONCHANNEL, "MODE :Target not in channel.");
	else
		channel.set_user_op(args[i], toggle);
}

static void	toggle_chan_limit(Client &author, Channel &channel, bool toggle, std::vector<std::string> &args)
{
	if (toggle && args.size() < 3)
		author.send_request(ERR_NEEDMOREPARAMS, "MODE :Missing limit param for mode 'l'.");
	if (!toggle)
		channel.set_limit(-1);
	else
	{
		int	nb;
		if (!::ft_atoi(args[2].c_str(), nb) || nb < 0)
			author.send_request(ERR_NEEDMOREPARAMS, "MODE :Bad argument for channel limit ('" + args[2] + "').");
		else
			channel.set_limit(nb);
	}
}

void	Mode(Client &client, Server &server, std::string &params)
{
	int		mode;
	bool	toggle;

	if (params.empty())
	{
		client.send_request(ERR_NEEDMOREPARAMS, "MODE :Need more params.");
		return;
	}
	
	std::vector<std::string> args = ft_split(params, ' ');

	std::list<Channel>				&channels = server.get_channels();
	std::list<Channel>::iterator	channel = std::find(channels.begin(), channels.end(), args[0]);
	
	if (channel == channels.end())
	{
		client.send_request(ERR_NOSUCHCHANNEL, "MODE :No such channels.");
		return;
	}

	if (args.size() == 1)
		client.send_request(RPL_CHANNELMODEIS, args[0] + Channel::mode_to_str(channel->get_mode(), channel->get_limit()));

	if (args[1][0] != '+' && args[1][0] != '-')
	{
		client.send_request(ERR_NEEDMOREPARAMS, "MODE :Missing toggle token.");
		return;
	}

	toggle = (args[1][0] == '+');

	for (std::size_t i = 1; i < args[1].size(); i++)
	{
		mode = parse_mode(args[1][i]);
		if (mode == -1)
		{
			client.send_request(ERR_UNKNOWNMODE, "MODE " + args[1].substr(i, 1) + " :Unknown mode.");
			return;
		}
		if (mode == CHAN_OPER)
			oper_chan_user(client, *channel, toggle, args);
		else if (mode == CHAN_MAX_USERS)
			toggle_chan_limit(client, *channel, toggle, args);
		else
			channel->toggle_mode((t_channel_mode) mode, toggle);
	}
	// todo send users in server new channel mode (or only in channel if channel secret mode is active)
}