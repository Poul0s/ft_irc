/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 18:40:44 by psalame           #+#    #+#             */
/*   Updated: 2024/05/10 13:06:05 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	this->_op = false;
	this->_status = SET_PASS;
}

Client::~Client()
{
}

void	Client::set_ip(std::string ip)
{
	this->_ip = ip;
}

void	Client::set_nickname(std::string nickname)
{
	this->_nickname = nickname;
}

void	Client::set_username(std::string username)
{
	this->_username = username;
}

void	Client::set_fd(int fd)
{
	this->_fd = fd;
}

void	Client::set_op(bool op)
{
	this->_op = op;
}

int	Client::get_fd(void)
{
	return this->_fd;
}

std::string	&Client::getCurrentReq(void)
{
	return this->_currentReq;
}
