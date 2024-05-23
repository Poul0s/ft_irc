/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:55:04 by psalame           #+#    #+#             */
/*   Updated: 2024/05/23 16:43:52 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_H
# define COMMANDS_H
# include "Server.hpp"

void	JoinChannel(Client &client, Server &server, std::string &params);
void	SendMsg(Client &client, Server &server, std::string &params);
void	Topic(Client &client, Server &server, std::string &params);

#endif