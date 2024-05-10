/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 18:42:34 by psalame           #+#    #+#             */
/*   Updated: 2024/05/10 14:21:00 by psalame          ###   ########.fr       */
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

void	Server::read_client_input(Client &client)
{
	char	buffer[1024];
	int		nb_read;
	std::string	&currentReq = client.getCurrentReq();

	do
	{
		nb_read = recv(client.get_fd(), buffer, 1023, MSG_DONTWAIT);
		if ((nb_read == -1 && errno != EAGAIN) || nb_read == 0)
			; // disconnect client
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
		currentReq = currentReq.substr(currentReq.find('\n') + 1);
		std::cout << "Client request: " << req << std::endl;
	}
}

#define POLL_FLAGS POLLIN | POLLRDHUP | POLLERR | POLLHUP

void	Server::runtime()
{
	struct pollfd	fds[NB_CLIENTS_MAX + 1];
	memset(fds, NB_CLIENTS_MAX + 1, sizeof(struct pollfd));
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
			if (revents & POLLIN)
				this->read_client_input(*it);
			if (revents & POLLOUT)
				std::cout << "for who ???" << std::endl;
			if (revents & ~(POLLIN | POLLOUT))
				std::cout << "cl close" << std::endl; // todo client close
		}
	}
}
