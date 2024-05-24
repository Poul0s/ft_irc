/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 13:13:11 by psalame           #+#    #+#             */
/*   Updated: 2024/05/24 10:43:57 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <list>
# include <set>
# include "Client.hpp"

typedef enum	e_channel_mode
{
	CHAN_MODE_PRIVATE = 1, // p
	CHAN_MODE_SECRET = 2, // s
	CHAN_MODE_INVITE_ONLY = 4, // i
	CHAN_MODE_TOPIC = 8, // t
	CHAN_MODE_MODERATED = 16, // m
	CHAN_MODE_NO_EXTERNAL_MSG = 32, // n
} t_channel_mode;

class Client;
class Channel
{
	public:
		Channel(const std::string &name);
		Channel(const std::string &name, const std::string &password);
		Channel(const Channel &cpy);
		~Channel();

		Channel	&operator=(const Channel &cpy);
		bool	operator==(const std::string &channelName) const;

		void				set_limit(int limit);
		void				set_topic(const std::string &topic);
		void				set_user_op(const std::string &client, bool toggle);
		void				set_user_op(const Client &client, bool toggle);
		void				toggle_mode(t_channel_mode mode, bool toggle);

		bool				is_full(void) const;
		bool				is_user_in(const std::string &client) const;
		bool				is_user_in(const Client &client) const;
		bool				is_user_banned(const std::string &client) const;
		bool				is_user_banned(const Client &client) const;
		bool				is_user_op(const std::string &client) const;
		bool				is_user_op(const Client &client) const;

		int					get_mode(void) const;
		bool				get_mode(t_channel_mode mode) const;
		const std::string	&get_topic(void) const;
		static std::string	mode_to_str(int mode);
		const std::string	&get_name() const;
		const std::string	&get_password() const;
		int					get_nb_users(void) const;

		void	add_user(Client &user);
		void	remove_user(Client &user);
		void	broadcast(const std::string &request, Client &sender);
		void	broadcast(const std::string &request);


	private:
		std::string	_name;
		std::string	_password;
		std::string	_topic;
		int			_limit;
		int			_mode;

		std::list<std::pair<Client *, bool> >	_usersIn; // first: client, second: isChanOperator
		std::set<std::string>					_bannedUsers;

		const std::string	get_channel_names(void) const;
};


#endif