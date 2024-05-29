/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 18:42:34 by psalame           #+#    #+#             */
/*   Updated: 2024/05/27 18:23:54 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "commands.h"
#include <algorithm>
#include "irc_error_codes.h"


Server::Server()
{
	this->_sockfd = -1;
}

Server::~Server()
{
	std::cout << "Closing server..." << std::endl;
	for (std::list<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		it->disconnect("Closing server.");
	}
	if (this->_sockfd != -1)
		::close(this->_sockfd);
}

void	Server::set_ip(std::string ip)
{
	this->_ip = ip;
}

void	Server::set_port(unsigned short port)
{
	this->_port = port;
}

void	Server::set_password(std::string password)
{
	this->_password = password;
}

std::list<Channel>	&Server::get_channels()
{
	return (this->_channels);
}

std::list<Client>	&Server::get_clients()
{
	return (this->_clients);
}

const std::string			&Server::get_ip() const
{
	return (this->_ip);
}

void	Server::create_socket()
{
	int					opt;
	int					sock_flags;
	struct sockaddr_in	addr;

	if (this->_sockfd != -1)
		throw std::runtime_error("Error: socket already created");
	this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_sockfd == -1)
		throw std::runtime_error("Error: socket creation failed");
	opt = 1;
	sock_flags = SO_REUSEADDR | SO_REUSEPORT;
	if (setsockopt(this->_sockfd, SOL_SOCKET, sock_flags, &opt, sizeof(opt)))
		throw std::runtime_error("Error: setsockopt failed");
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY; //inet_addr(this->_ip.c_str());
	addr.sin_port = htons(this->_port);
	if (bind(this->_sockfd, (struct sockaddr *)&addr, sizeof(addr)))
		throw std::runtime_error("Error: bind failed");
	if (listen(this->_sockfd, NB_CLIENTS_MAX))
		throw std::runtime_error("Error: listen failed");
	fcntl(this->_sockfd, F_SETFL, O_NONBLOCK);
}

void	Server::accept_client()
{
	int					client_sock;
	struct sockaddr_in	client_adress;
	unsigned int		addr_len;

	addr_len = sizeof(client_adress);
	client_sock = accept(this->_sockfd, (struct sockaddr *)&client_adress, &addr_len);
	if (client_sock != -1)
	{
		char	ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(client_adress.sin_addr), ip, INET_ADDRSTRLEN);

		Client	client (*this);
		client.set_ip(ip);
		client.set_fd(client_sock);
		this->_clients.push_back(client);
	}
}

void	Server::clean_clients()
{
	std::list<Client>::iterator	it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		if (it->get_fd() == -1)
		{
			std::cout << "Cleaning client " << it->get_nickname() << std::endl;
			for (std::list<Channel>::iterator it_chan = this->_channels.begin(); it_chan != this->_channels.end(); it_chan++)
				it_chan->remove_user(*it);
			it = this->_clients.erase(it);
		}
		else
			it++;
	}
}

void	Server::process_command(Client &client, std::string &req)
{
	std::cout << "Processing command: " << req << std::endl;
	std::string	command = req.substr(0, req.find(' '));
	std::transform(command.begin(), command.end(), command.begin(), &toupper);
	std::string	params = req.substr(std::min(req.find_first_not_of(' ', std::min(req.find(' '), req.size())), req.size()));
	
	// maybe set a map with command as key and function as value
	if (command == "JOIN")
		JoinChannel(client, *this, params);
	else if (command == "PRIVMSG")
		SendMsg(client, *this, params);
	else if (command == "TOPIC")
		Topic(client, *this, params);
	else if (command == "LIST")
		List(client, *this, params);
	else if (command == "KICK")
		Kick(client, *this, params);
	else if (command == "KILL")
		Kill(client, *this, params);
	else if (command == "OPER")
		Oper(client, *this, params);
	else if (command == "NICK")
		Nick(client, *this, params);
	else if (command == "QUIT")
		Quit(client, *this, params);
	else if (command == "PART")
		Part(client, *this, params);
}

void	Server::process_request(Client &client, std::string &req)
{
	if (client.get_fd() == -1 || client.get_status() == DISCONNECTED)
		return ;
	switch (client.get_status())
	{
		case IDENTIFIED:
			this->process_command(client, req);
			break;
		case GET_FORMAT:
			if (req == "CAP LS 302")
				client.set_status(SET_PASS);
			else
				client.disconnect("bad format (got [" + req + "])");
			break;
		case SET_PASS:
			if (req.rfind("PASS", 0) == 0)
			{
				if (req == "PASS " + this->_password || req == "PASS :" + this->_password)
					client.set_status(SET_NICK);
				else
					client.disconnect("bad password");
			}
			else
				client.disconnect("bad request (asking PASS)");
			break;
		case SET_NICK:
			if (req.rfind("NICK", 0) == 0)
			{
				req = req.substr(std::min(req.find_first_not_of(' ', 5), req.size()));
				Nick(client, *this, req);
			}
			else
				client.disconnect("bad request (asking NICK)");
			break;
		case SET_USER:
			if (req.rfind("USER", 0) != 0)
				client.disconnect("bad request (asking USER)");
			else if (req.find_first_of(' ', 5) == std::string::npos || req.find_first_of(':') == std::string::npos)
				client.disconnect("bad USER request format (asking USER user 0 * :realname)");
			else
			{
				std::string	user = req.substr(5, req.find_first_of(' ', 5) - 5);
				std::string	realname = req.substr(req.find_first_of(':'), req.size() - 1);
				if (user.size() > 15 || realname.size() > 15)
					client.disconnect("invalid user size");
				client.set_username(user);
				client.set_realname(realname);
				client.set_status(IDENTIFIED);
				std::cout << "New client : " << std::endl << "-IP: " << client.get_ip() << std::endl << "-Nick: " << client.get_nickname() << std::endl << "-User: " << client.get_username() << std::endl << "-Realname: " << client.get_realname() << std::endl;
				// todo maybe send all channels mode
			}
			break;
		default:
			client.disconnect("invalid command");
	}
}

void	Server::read_client_input(Client &client)
{
	char	buffer[1024];
	int		nb_read;
	std::string	&currentReq = client.getCurrentReq();

	std::cout << "Reading client input" << currentReq << std::endl;
	do
	{
		nb_read = recv(client.get_fd(), buffer, 1023, MSG_DONTWAIT);
		if ((nb_read == -1 && errno != EAGAIN) || nb_read == 0)
		{
			if (client.get_status() == IDENTIFIED)
				this->broadcast(":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + " QUIT :user disconnected with no reason");
			client.disconnect("user disconnected");
			return ;
		}
		else if (nb_read > 0)
		{
			buffer[nb_read] = 0;
			currentReq += buffer;
			std::cout << currentReq << std::endl;
		}
	} while (nb_read > 0);
	while (currentReq.find('\n') != std::string::npos)
	{
		std::string	req;
		req = currentReq.substr(0, currentReq.find('\n'));
		if (req[req.size() - 1] == '\r')
			req = req.substr(0, req.size() - 1);
		this->process_request(client, req);
		currentReq = currentReq.substr(currentReq.find('\n') + 1);
	}
}

#define POLL_FLAGS POLLIN | POLLRDHUP | POLLERR | POLLHUP

void	Server::runtime()
{
	struct pollfd	fds[NB_CLIENTS_MAX + 1];
	memset(fds, 0, (NB_CLIENTS_MAX + 1) * sizeof(struct pollfd));
	fds[0].fd = this->_sockfd;
	fds[0].events = POLL_FLAGS;

	std::cout << "Start listening" << std::endl;

	while (this->_sockfd != -1)
	{
		std::list<Client>::iterator	end_client_poll = this->_clients.end();
		for (std::list<Client>::iterator it = this->_clients.begin(); it != end_client_poll; it++)
		{
			fds[std::distance(this->_clients.begin(), it) + 1].fd = it->get_fd();
			fds[std::distance(this->_clients.begin(), it) + 1].events = POLL_FLAGS;
		}
		int nbAccess = poll(fds, this->_clients.size() + 1, 2147483647);
		if (nbAccess < 0)
		{
			if (errno == EINTR)
				return ;
			throw std::runtime_error("Error: poll failed: " + std::string(strerror(errno)));
		}
		std::cout << "poll released for " << nbAccess << " fds." << std::endl;
		if (nbAccess == 0)
			continue ;
		if (fds[0].revents)
		{
			if (fds[0].revents & POLLIN)
				this->accept_client();
			if (fds[0].revents & POLLOUT)
				std::cout << "for what ?" << std::endl;
			if (fds[0].revents & ~(POLLIN | POLLOUT))
				throw std::runtime_error("Server socket closed.");
		}
		for (std::list<Client>::iterator it = this->_clients.begin(); it != end_client_poll; it++)
		{
			short	revents = fds[std::distance(this->_clients.begin(), it) + 1].revents;
			if (revents && revents & POLLIN)
				this->read_client_input(*it);
			if (revents && revents & POLLOUT)
				std::cout << "for who ???" << std::endl;
			if (revents && revents & ~(POLLIN | POLLOUT))
			{
				if (it->get_status() == IDENTIFIED)
					this->broadcast(":" + it->get_nickname() + "!" + it->get_username() + "@" + it->get_ip() + " QUIT :user disconnected");
				it->disconnect("user disconnected");
			}
		}
		this->clean_clients();
	}
}

void	Server::add_channel(const Channel &channel)
{
	this->_channels.push_back(channel);

	if (!channel.get_mode(CHAN_MODE_SECRET))
	{
		std::string	request;
		request = ":" + this->_ip + " MODE " + channel.get_name() + " +" + Channel::mode_to_str(channel.get_mode());
		this->broadcast(request);
	}
}

void	Server::broadcast(const std::string &request)
{
	for (std::list<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		it->send_request(request);
}
