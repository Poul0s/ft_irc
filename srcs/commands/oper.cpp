/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:04:32 by psalame           #+#    #+#             */
/*   Updated: 2024/05/27 17:13:20 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "irc_error_codes.h"

void	Oper(Client &client, Server &server, std::string &params)
{
	if (params.find(' ') == std::string::npos)
	{
		client.send_request(ERR_NEEDMOREPARAMS, "OPER :Not enough parameters");
		return ;
	}
	std::string	operName = params.substr(0, params.find(' '));
	std::string	operPassword = params.substr(params.find(' ') + 1);
	if (operName.empty() || operPassword.empty())
	{
		client.send_request(ERR_NEEDMOREPARAMS, "OPER :Not enough parameters");
		return ;
	}
	if (operName != "admin" && operPassword != "admin") // todo fix: if any of these is correct but the other one is not, still work
	{
		client.send_request(ERR_PASSWDMISMATCH, "OPER :Password incorrect");
		return ;
	}
	client.set_op(true);
	client.send_request(RPL_YOUREOPER, "OPER :You are now an IRC operator");
	server.broadcast(":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + " MODE " + client.get_nickname() + " +o");
}