/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 14:06:39 by psalame           #+#    #+#             */
/*   Updated: 2024/05/24 07:33:38 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "irc_error_codes.h"
#include <algorithm>
#include <sstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void	SendChannelMsg(Client &client, Server &server, std::string &channelName, std::string &message)
{
	std::list<Channel>				&channels = server.get_channels();
	std::list<Channel>::iterator	channel = std::find(channels.begin(), channels.end(), channelName);

	if (channel == channels.end())
		client.send_request(ERR_CANNOTSENDTOCHAN, "PRIVMSG :Cannot send to channel");
	else
	{
		if (channel->get_mode(CHAN_MODE_NO_EXTERNAL_MSG) && !channel->is_user_in(client))
			client.send_request(ERR_CANNOTSENDTOCHAN, "PRIVMSG :Cannot send to channel");
		else if (channel->is_user_banned(client))
			client.send_request(ERR_CANNOTSENDTOCHAN, "PRIVMSG :Cannot send to channel");
		else
		{
			std::string	request = ":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + " PRIVMSG " + channelName + " :" + message;
			channel->broadcast(request, client);
		}
	}
}

void	SendUserMsg(Client &client, Server &server, std::string &userName, std::string &message)
{
	std::list<Client>	&clients = server.get_clients();
	std::list<Client>::iterator	clientIt = std::find(clients.begin(), clients.end(), userName);

	if (clientIt == clients.end())
		client.send_request(ERR_NOSUCHNICK, "PRIVMSG :No such nick");
	else
	{
		std::string	request = ":" + client.get_nickname() + "!" + client.get_username() + "@" + client.get_ip() + " PRIVMSG " + userName + " :" + message;
		clientIt->send_request(request);
	}
}

#include <sys/wait.h>

std::string err(std::string s)
{
	std::cout << s << std::endl;
	return s;
}

std::string	curlRequest(char *av[]) {
	int			status;
	int			fd[2], fderr[2];
	char		buf[1025] = {0};
	std::string		res;
	char			*env[] = {
		(char *) 0
	};

	if (pipe(fd) == -1 || pipe(fderr) == -1)
		return err("error: fatal\n");
		
	int pid = fork();
	if (!pid) {
		if (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1
			|| dup2(fderr[1], 2) == -1 || close(fderr[0]) == -1 || close(fderr[1]) == -1)
			return err("error: fatal\n");
		execve(av[0], av, env);
		err("error: cannot execute " + std::string(av[0]) + "\n");
		exit(1);
	} else {
		waitpid(pid, &status, 0);
		close(fd[1]);
		close(fderr[1]);
		size_t	readed;
		while ((readed = read(fd[0], buf, 1024)) > 0)
		{
			buf[readed] = 0;
			res += buf;
		}
		close(fd[0]);
		while (read(fderr[0], buf, 1024) > 0)
			;
		close(fderr[0]);
		return res;
	}
	return res;
}

char verifyn(char c)
{
	if (c == '\n')
		c = ' ';
	return c;
}

void	ProcessBot(Client &client, Server &server, std::string &message)
{
	client.add_bot_content("user", message);

	json req = json::parse(R"({
		"model": "gpt-3.5-turbo"
	})");
	req["messages"] = client.get_bot_history();
	std::string json_str = req.dump();
	char *gpt[] = {
		(char *) "/bin/curl",
		(char *) "https://api.openai.com/v1/chat/completions",
		(char *) "-H", (char *) "Content-Type: application/json",
		(char *) "-H", (char *) (std::string("Authorization: Bearer ") + std::string(GPT_TOKEN)).c_str(),
		(char *) "-d", (char *) json_str.c_str(),
		(char *) 0
	};
	std::string response_str = curlRequest(gpt);
	json response = json::parse(response_str);
	std::string response_msg;
	if (response.contains("choices") && response["choices"].is_array() && !response["choices"].empty() &&
		response["choices"][0].contains("message") && response["choices"][0]["message"].contains("content") &&
		response["choices"][0]["message"]["content"].is_string())
	{
		response_msg = response["choices"][0]["message"]["content"];
		client.add_bot_content("system", response_msg);
	}
	else
		response_msg = "Error while getting bot response.";

	std::transform(response_msg.begin(), response_msg.end(), response_msg.begin(), &verifyn);

	std::string	request = ":Bot!bot@" + server.get_ip() + " PRIVMSG " + client.get_username() + " :" + response_msg;
	client.send_request(request);
}

void	SendMsg(Client &client, Server &server, std::string &params)
{
	if (params.find(' ') == std::string::npos)
	{
		client.send_request(ERR_NORECIPIENT, "PRIVMSG :No Recipient given");
		return ;
	}
	if (params.find(':', params.find(' ')) == std::string::npos)
	{
		client.send_request(ERR_NOTEXTTOSEND, "PRIVMSG :No text to send");
		return ;
	}
	std::string	destinations = params.substr(0, params.find(' '));
	std::string	message = params.substr(params.find(':', params.find(' ')) + 1);

	while (!destinations.empty())
	{
		std::string	destination = destinations.substr(0, destinations.find(','));
		if (destinations.find(',') != std::string::npos)
			destinations = destinations.substr(destinations.find(',') + 1);
		else
			destinations = "";
		if (destination[0] == '#' || destination[0] == '&')
			SendChannelMsg(client, server, destination, message);
		else if (destination == "Bot")
			ProcessBot(client, server, message);
		else
			SendUserMsg(client, server, destination, message);
	}
}