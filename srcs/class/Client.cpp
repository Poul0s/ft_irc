/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 18:40:44 by psalame           #+#    #+#             */
/*   Updated: 2024/05/24 14:52:47 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <algorithm>
#include <utils.h>

Client::Client(const Server &attachedServer) : _attachedServer(attachedServer)
{
	this->_op = false;
	this->_status = GET_FORMAT;
}

Client::~Client()
{
}

bool	Client::operator==(const std::string &clientNickname) const
{
	return this->_nickname == clientNickname;
}

void	Client::set_ip(std::string ip)
{
	this->_ip = ip;
}

void	Client::set_nickname(std::string nickname)
{
	this->_nickname = nickname;
}

void	Client::set_failNickname(std::string nickname)
{
	this->_failNickname = nickname;
}

void	Client::set_username(std::string username)
{
	this->_username = username;
}

void	Client::set_realname(std::string realname)
{
	this->_realname = realname;
}

void	Client::set_fd(int fd)
{
	this->_fd = fd;
}

void	Client::set_op(bool op)
{
	this->_op = op;
}

void	Client::set_status(int status)
{
	this->_status = status;
}

bool	Client::is_op(void) const
{
	return this->_op;
}

int	Client::get_fd(void)
{
	return this->_fd;
}

int	Client::get_status(void)
{
	return this->_status;
}

std::string	&Client::getCurrentReq(void)
{
	return this->_currentReq;
}

std::string	&Client::get_ip(void)
{
	return this->_ip;
}

std::string	&Client::get_nickname(void)
{
	return this->_nickname;
}

const std::string	&Client::get_nickname(void) const
{
	return this->_nickname;
}

std::string	&Client::get_failNickname(void)
{
	return this->_failNickname;
}

const std::string	&Client::get_failNickname(void) const
{
	return this->_failNickname;
}

std::string	&Client::get_username(void)
{
	return this->_username;
}

std::string	&Client::get_realname(void)
{
	return this->_realname;
}

void		Client::send_request(std::string request) const
{
	if (request.empty())
		return ;
	if (request[request.size() - 1] != '\n')
		request += '\n';
	send(this->_fd, request.c_str(), request.size(), MSG_NOSIGNAL | MSG_DONTWAIT);
}

void		Client::send_request(int code, std::string data) const
{
	std::string	request;

	request = ":" + this->_attachedServer.get_ip() + " " + ft_itoa(code) + " " + this->_nickname + " " + data;
	this->send_request(request);
}

void	Client::disconnect(std::string reason)
{
	std::cout << "Disconnecting client " << this->_nickname << " for " << reason << std::endl;
	if (this->_fd != -1)
	{
		this->send_request("You have been kicked from server for " + reason + ".");
		close(this->_fd);
	}
	this->_status = DISCONNECTED;
	this->_fd = -1;
}

