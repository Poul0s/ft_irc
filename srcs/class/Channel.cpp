/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 13:16:11 by psalame           #+#    #+#             */
/*   Updated: 2024/05/22 14:15:39 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "irc_error_codes.h"
#include <algorithm>

Channel::Channel(const std::string &name)
{
	this->_name = name;
	this->_limit = -1;
}

Channel::Channel(const std::string &name, const std::string &password)
{
	this->_name = name;
	this->_password = password;
	this->_limit = -1;
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
	this->_limit = -1;
	return (*this);
}

bool	Channel::operator==(const std::string &channelName) const
{
	return (this->_name == channelName);
}

bool			Channel::is_full(void) const
{
	return (this->_limit != -1 && this->_usersIn.size() >= static_cast<unsigned long>(this->_limit));
}

bool			Channel::is_user_banned(const std::string &client) const
{
	return (std::find(this->_bannedUsers.begin(), this->_bannedUsers.end(), client) != this->_bannedUsers.end());
}

bool			Channel::is_user_banned(const Client &client) const
{
	return (this->is_user_banned(client.get_nickname()));
}

const std::string	&Channel::get_name() const
{
	return this->_name;
}

const std::string	&Channel::get_password() const
{
	return this->_password;
}

typedef std::pair<Client &, bool> userIn_t;

const std::string	Channel::get_channel_names(void) const
{
	std::string	names;

	for (std::list<userIn_t>::const_iterator it = this->_usersIn.begin(); it != this->_usersIn.end(); it++)
	{
		if (it != this->_usersIn.begin())
			names += ' ';
		if (it->second)
			names += '@';
	}
	return names;
}

void	Channel::add_user(Client &client)
{
	for (std::list<userIn_t>::iterator it = this->_usersIn.begin(); it != this->_usersIn.end(); it++)
		if (it->first == client.get_nickname())
			return ;

	this->_usersIn.push_back(userIn_t(client, false));
	
	std::string	request = ":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip();
	request += " JOIN :" + this->_name;
	for (std::list<userIn_t>::iterator it = this->_usersIn.begin(); it != this->_usersIn.end(); it++)
			it->first.send_request(request);

	//  (todo) send mode
	//  (todo) send RPL_TOPIC and RPL_TOPICTIME if topic is set (code 332 and 333)
	//  send users in channel (code 353 and 366)
	client.send_request(RPL_NAMREPLY, "@ " + this->_name + " :" + this->get_channel_names());
	client.send_request(RPL_ENDOFNAMES, this->_name + " :End of /NAMES list.");
}
