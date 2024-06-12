/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycontre <ycontre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:24:37 by ycontre           #+#    #+#             */
/*   Updated: 2024/06/12 16:21:27 by ycontre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "irc_error_codes.h"
#include <algorithm>
#include "utils.h"
#include <vector>

void	Invite(Client &client, Server &server, std::string &params)
{
	if (params.empty())
	{
		client.send_request(ERR_NEEDMOREPARAMS, "INVITE :Need more params.");
		return;
	}

	std::vector<std::string> args = ft_split(params, ' ');
	if (args.size() < 2)
	{
		client.send_request(ERR_NEEDMOREPARAMS, "INVITE :Need more params.");
		return;
	}

	std::list<Channel> &channels = server.get_channels();
	std::list<Channel>::iterator it_channel = std::find(channels.begin(), channels.end(), args[1]);
	if (it_channel == channels.end() || !it_channel->is_user_in(client))
	{
		client.send_request(ERR_NOTONCHANNEL, args[1] + " :Not on channel.");
		return;
	}
	if(!it_channel->is_user_op(client))
	{
		client.send_request(ERR_CHANOPRIVSNEEDED, it_channel->get_name() + " :You are not operator.");
		return;
	}
	if(it_channel->is_user_in(args[0]))
	{
		client.send_request(ERR_USERONCHANNEL, args[0] + " " + it_channel->get_name() + " :User already in channel.");
		return;
	}

	std::list<Client> &clients = server.get_clients();
	std::list<Client>::iterator it_client = std::find(clients.begin(), clients.end(), args[0]);
	if (it_client == clients.end())
	{
		client.send_request(ERR_NOSUCHNICK, args[0] + " :User disconnected.");
		return;
	}
	
	it_channel->add_invitation(args[0]);

	client.send_request(RPL_INVITING, args[0] + " " + it_channel->get_name());
	
	std::string	request = ":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + " INVITE " + args[0] + " " + args[1];
	it_client->send_request(request);
	
	// it_channel.broadcast();
}