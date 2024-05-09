/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 18:14:32 by psalame           #+#    #+#             */
/*   Updated: 2024/05/09 18:25:37 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <stdbool.h>
#include <asm-generic/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <list>
#include <iostream>


bool	create_socket(int *sockfd)
{
	int	opt;
	int	sock_flags;

	*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (*sockfd == -1)
		return (false);
	opt = 1;
	sock_flags = SO_REUSEADDR | SO_REUSEPORT;
	if (setsockopt(*sockfd, SOL_SOCKET, sock_flags, &opt, sizeof(opt)))
		return (false);
	return (true);
}

void	try_accept_client(int sockfd, std::list<int> &clients)
{
	int					client_sock;
	struct sockaddr_in	client_adress;
	unsigned int		addr_len;
	char				ip[INET_ADDRSTRLEN];

	addr_len = sizeof(client_adress);
	client_sock = accept(sockfd, (struct sockaddr *) &client_adress, &addr_len);
	if (client_sock != -1)
	{
		inet_ntop(AF_INET, &(client_adress.sin_addr), ip, INET_ADDRSTRLEN);
		clients.push_back(client_sock);
	}
}

int	main(void)
{
	struct sockaddr_in	addr;
	char				ip[] = "127.0.0.1";
	int					sockfd;

	if (!create_socket(&sockfd))
		return 1;
	if (inet_aton(ip, &(addr.sin_addr)) == 0)
		return 1;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(42420);
	if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) != 0)
		return 1;
	if (listen(sockfd, 2147483647) != 0)
		return 1;
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	
	std::list<int>	clients;
	while (1)
	{
		try_accept_client(sockfd, clients);
		for (std::list<int>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			char	buff[255];
			buff[0] = 0;
			int rd = read(*it, buff, 255);
			if (rd > 0)
			{
				buff[rd] = 0;
				std::cout << buff << std::endl;
			}
		}
	}
}