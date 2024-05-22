/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 13:13:11 by psalame           #+#    #+#             */
/*   Updated: 2024/05/22 11:01:34 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <pair>
# include "Client.hpp"

class Channel
{
	public:
		Channel(const std::string &name);
		Channel(const std::string &name, const std::string &password);
		Channel(const Channel &cpy);
		~Channel();

		Channel	&operator=(const Channel &cpy);
		bool	operator==(const std::string &channelName) const;

		const std::string	&get_name() const;
		const std::string	&get_password() const;

		void	add_user(Client &user);

	private:
		std::string	_name;
		std::string	_password;

		std::list<std::pair<Client &, bool> >	_usersIn; // first: client, second: isOperator

		const std::string	Channel::get_channel_names(void) const
};


#endif