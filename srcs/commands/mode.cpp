/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:13:38 by ycontre           #+#    #+#             */
/*   Updated: 2024/06/18 14:54:42 by psalame          ###   ########.fr       */
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
	std::string		modes = "opsitmnlk";
	std::size_t		pos;

	pos = modes.find(c);
	if (pos == std::string::npos)
		return (-1);
	return (std::pow(2, static_cast<int>(pos) - 1));
}

static void	toggle_chan_limit(Client &author, Channel &channel, bool toggle, std::vector<std::string> &args)
{
	if (toggle && args.size() < 3)
		author.send_request(ERR_NEEDMOREPARAMS, "MODE :Missing limit param for mode 'l'.");
	else if (!toggle)
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

static void	oper_chan_user(Client &author, Channel &channel, bool toggle, std::vector<std::string> &args)
{
	std::size_t	i;

	i = args[1].find('l') == std::string::npos ? 2 : 3;
	if (args.size() <= i)
		author.send_request(ERR_NEEDMOREPARAMS, "MODE :Missing user param for mode 'o'.");
	else if (!channel.is_user_in(args[i]))
		author.send_request(ERR_NOTONCHANNEL, "MODE :Target not in channel.");
	else
		channel.set_user_op(args[i], toggle);
}


static void	set_chan_keyword(Client &author, Channel &channel, bool toggle, std::vector<std::string> &args)
{
	std::size_t	i;

	if (!toggle)
	{
		channel.set_password("");
	}
	else
	{
		i = 2;
		if (args[1].find('l') != std::string::npos)
			i++;
		if (args[1].find('o') != std::string::npos)
			i++;
		if (args.size() <= i)
			author.send_request(ERR_NEEDMOREPARAMS, "MODE :Missing user param for mode 'k'.");
		else
			channel.set_password(args[i]);
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
	{
		client.send_request(RPL_CHANNELMODEIS, args[0] + " " + Channel::mode_to_str(channel->get_mode(), channel->get_limit()));
		return ;
	}

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
		else if (mode == CHAN_KEYWORD)
			set_chan_keyword(client, *channel, toggle, args);
		else
			channel->toggle_mode((t_channel_mode) mode, toggle);
	}
	
	std::string request = ":" + server.get_ip() + " MODE " + channel->get_name() + " +" + Channel::mode_to_str(channel->get_mode(), channel->get_limit());
	if (channel->get_mode(CHAN_MODE_SECRET))
		channel->broadcast(request);
	else
		server.broadcast(request);
}