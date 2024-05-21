/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 18:08:32 by psalame           #+#    #+#             */
/*   Updated: 2024/05/21 13:37:37 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <list>

enum e_status
{
	GET_FORMAT,
	SET_PASS,
	SET_NICK,
	SET_USER,
	IDENTIFIED
};

class	Client {
	public:
		Client();
		~Client();

		void	set_ip(std::string ip);
		void	set_nickname(std::string nickname);
		void	set_username(std::string username);
		void	set_realname(std::string realname);

		void	set_fd(int fd);
		void	set_op(bool op);
		void	set_status(int status);

		int			get_fd(void);
		int			get_status(void);
		std::string	&getCurrentReq(void);

		std::string	&get_ip(void);
		std::string	&get_nickname(void);
		std::string	&get_username(void);
		std::string	&get_realname(void);

		void		send_request(std::string request) const;
		void		send_request(std::string server_ip, int code, std::string data) const;

		void		disconnect(std::string reason);

	private:
		std::string	_ip;
		std::string	_nickname; // unique in server
		std::string	_username;
		std::string	_realname;
		std::string	_currentReq;

		int			_fd;
		bool		_op;
		int			_status;
};

#endif