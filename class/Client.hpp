/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 18:08:32 by psalame           #+#    #+#             */
/*   Updated: 2024/05/10 11:44:39 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

enum e_status
{
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

		void	set_fd(int fd);
		void	set_op(bool op);


		int			get_fd(void);
		std::string	&getCurrentReq(void);

	private:
		std::string	_ip;
		std::string	_nickname;
		std::string	_username;
		std::string	_currentReq;

		int			_fd;
		bool		_op;
		int			_status;
};

#endif