/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 13:16:11 by psalame           #+#    #+#             */
/*   Updated: 2024/05/21 14:00:21 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const std::string &name)
{
	this->_name = name;
}

Channel::Channel(const std::string &name, const std::string &password)
{
	this->_name = name;
	this->_password = password;
}

Channel::Channel(const Channel &cpy)
{
	*this = cpy;
}

Channel::~Channel()
{
}

Channel	&Channel::operator=(const Channel &cpy)
{
	this->_name = cpy._name;
	this->_password = cpy._password;
	return (*this);
}

bool	Channel::operator==(const std::string &channelName) const
{
	return (this->_name == channelName);
}

const std::string	&Channel::get_name() const
{
	return this->_name;
}

void	Channel::add_user(Client &client)
{
	this->_usersIn.push_back(client);
	// send all users the requests
}
