/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 18:42:34 by psalame           #+#    #+#             */
/*   Updated: 2024/05/13 17:48:39 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
	this->_sockfd = -1;
}

Server::~Server()
{
	// disconnect all clients
	if (this->_sockfd != -1)
		close(this->_sockfd);
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
	addr.sin_addr.s_addr = inet_addr(this->_ip.c_str());
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

		Client	client;
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
			it = this->_clients.erase(it);
		else
			it++;
	}
}

void	Server::process_command(Client &client, std::string &req)
{
	(void)client;
	std::cout << "Processing command: " << req << std::endl;
}

void	Server::process_request(Client &client, std::string &req)
{
	if (client.get_fd() == -1)
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
				if (req == "PASS " + this->_password + "")
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
				std::string	nick = req.substr(5, req.size() - 5);
				std::cout << "Nick" << ":" << nick << std::endl;
				if (nick.size() > 15)
					client.disconnect("invalid nickname size");
				client.set_nickname(nick);
				client.set_status(SET_USER);
			}
			else
				client.disconnect("bad request (asking NICK)");
			break;
		case SET_USER:
			if (req.rfind("USER", 0) == 0)
			{
				std::string	user = req.substr(5, req.find_first_of(' ', 5) - 5);
				std::string	realname = req.substr(req.find_first_of(':'), req.size() - 1);
				if (user.size() > 15 || realname.size() > 15)
					client.disconnect("invalid user size");
				client.set_username(user);
				client.set_realname(realname);
				client.set_status(IDENTIFIED);
				std::cout << "New client : " << std::endl << "-IP: " << client.get_ip() << std::endl << "-Nick: " << client.get_nickname() << std::endl << "-User: " << client.get_username() << std::endl << "-Realname: " << client.get_realname() << std::endl;
			}
			else
				client.disconnect("bad request (asking USER)");
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
		std::cout << "poll released for " << nbAccess << " fds." << std::endl;
		if (nbAccess < 0)
			throw std::runtime_error("Error: poll failed: " + std::string(strerror(errno)));
		else if (nbAccess == 0)
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
				it->disconnect("user disconnected");
		}
		this->clean_clients();
	}
}
