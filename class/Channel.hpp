/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 13:13:11 by psalame           #+#    #+#             */
/*   Updated: 2024/05/21 13:55:14 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
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

		std::list<Client &>	_usersIn;

};


#endif