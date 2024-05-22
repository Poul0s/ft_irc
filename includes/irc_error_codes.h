/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc_error_codes.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:20:34 by psalame           #+#    #+#             */
/*   Updated: 2024/05/22 14:04:03 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_ERROR_CODES_H
# define IRC_ERROR_CODES_H
// https://alien.net.au/irc/irc2numerics.html

# define RPL_NAMREPLY 353
# define RPL_ENDOFNAMES 366
# define ERR_NOSUCHCHANNEL 403
# define ERR_NEEDMOREPARAMS 461
# define ERR_CHANNELISFULL 471
# define ERR_BANNEDFROMCHAN 474
# define ERR_BADCHANNELKEY 475

#endif