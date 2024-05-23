/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_error_codes.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:20:34 by psalame           #+#    #+#             */
/*   Updated: 2024/05/23 17:55:31 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_ERROR_CODES_H
# define IRC_ERROR_CODES_H
// https://alien.net.au/irc/irc2numerics.html

# define RPL_NOTOPIC 331
# define RPL_TOPIC 332
# define RPL_TOPICTIME 333
# define RPL_NAMREPLY 353
# define RPL_ENDOFNAMES 366
# define ERR_NOSUCHNICK 401
# define ERR_NOSUCHCHANNEL 403
# define ERR_CANNOTSENDTOCHAN 404
# define ERR_NORECIPIENT 411
# define ERR_NOTEXTTOSEND 412
# define ERR_UNKNOWNCOMMAND 421
# define ERR_NOTONCHANNEL 442
# define ERR_NEEDMOREPARAMS 461
# define ERR_CHANNELISFULL 471
# define ERR_BANNEDFROMCHAN 474
# define ERR_BADCHANNELKEY 475
# define ERR_CHANOPRIVSNEEDED 482

#endif