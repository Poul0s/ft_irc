/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:22:07 by psalame           #+#    #+#             */
/*   Updated: 2024/05/24 10:22:18 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <vector>
# include <string>
# include <limits>
# include <cstdlib>

std::vector<std::string>	ft_split(std::string str, char sep);
std::string					ft_itoa(int code);

template<class T>
bool	ft_atoi(const char *str, T &res)
{
	char	*endptr;
	long	nb;

	if (*str == '\0')
		return (false);
	errno = 0;
	nb = std::strtol(str, &endptr, 10);
	if (*endptr != '\0' || errno == ERANGE || nb < std::numeric_limits<T>::min() || nb > std::numeric_limits<T>::max())
		return (false);
	res = static_cast<T>(nb);
	return (true);
}

#endif