/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:54:27 by psalame           #+#    #+#             */
/*   Updated: 2024/05/24 14:56:27 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void	Quit(Client &client, Server &server, std::string &params)
{
	(void)params;
	params = params.substr(std::min(params.find(':'), params.size() - 1) + 1);
	if (params.empty())
		params = "Client Quit";
	std::string	request = ":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + " QUIT :" + params;
	server.broadcast(request);
	client.disconnect(params);
}