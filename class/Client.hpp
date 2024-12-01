/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 18:08:32 by psalame           #+#    #+#             */
/*   Updated: 2024/05/24 14:51:12 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include <string>
# include <list>
# include <nlohmann/json.hpp>

enum e_status
{
	GET_FORMAT,
	SET_PASS,
	SET_ID,
	// SET_NICK,
	// SET_USER,
	IDENTIFIED,
	DISCONNECTED
};

class	Server;
class	Client {
	public:
		Client(const Server &attachedServer);
		~Client();

		bool	operator==(const std::string &clientNickname) const;

		void	set_ip(std::string ip);
		void	set_nickname(std::string nickname);
		void	set_failNickname(std::string nickname);
		void	set_username(std::string username);
		void	set_realname(std::string realname);

		void	set_fd(int fd);
		void	set_op(bool op);
		void	set_status(int status);

		bool		is_op(void) const;

		int			get_fd(void);
		int			get_status(void);
		std::string	&getCurrentReq(void);

		std::string	&get_ip(void);
		std::string	&get_nickname(void);
		const std::string	&get_nickname(void) const;
		std::string	&get_failNickname(void);
		const std::string	&get_failNickname(void) const;
		std::string	&get_username(void);
		std::string	&get_realname(void);

		void		send_request(std::string request) const;
		void		send_request(int code, std::string data) const;

		void		disconnect(std::string reason);

		const nlohmann::json	&get_bot_history() const;
		void					add_bot_content(const std::string &role, const std::string &content);

	private:
		std::string	_ip;
		std::string	_nickname; // unique in server
		std::string	_failNickname;
		std::string	_username;
		std::string	_realname;
		std::string	_currentReq;

		int			_fd;
		bool		_op;
		int			_status;

		const Server	&_attachedServer;

		nlohmann::json	_botHistory;
};

#endif