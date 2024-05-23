/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 18:27:54 by psalame           #+#    #+#             */
/*   Updated: 2024/05/23 18:37:17 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "irc_error_codes.h"
#include <sstream>
#include <list>
#include <algorithm>

static std::list<std::string>	ft_split(std::string str, char sep)
{
	std::list<std::string>	result;
	std::string				tmp;

	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (*it == sep)
		{
			result.push_back(tmp);
			tmp.clear();
		}
		else
			tmp += *it;
	}
	if (!tmp.empty())
		result.push_back(tmp);
	return (result);

}

void	List(Client &client, Server &server, std::string &params)
{
	std::list<std::string>	arguments = ft_split(params, ' ');

	client.send_request(RPL_LISTSTART, "Channel :Users Name");
	for (std::list<Channel>::iterator it = server.get_channels().begin(); it != server.get_channels().end(); it++)
	{
		if (arguments.empty() || std::find(arguments.begin(), arguments.end(), it->get_name()) != arguments.end())
		{
			if (!it->get_mode(CHAN_MODE_SECRET) || it->is_user_in(client))
			{
				std::stringstream ss;
				ss << it->get_nb_users();
				client.send_request(RPL_LIST, it->get_name() + " " + ss.str() + " :" + it->get_topic());
			}
		}
	}
	client.send_request(RPL_LISTEND, "End of LIST");
}