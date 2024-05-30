/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psalame <psalame@student.42angouleme.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 10:20:55 by psalame           #+#    #+#             */
/*   Updated: 2024/05/24 10:21:19 by psalame          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>

std::vector<std::string>	ft_split(std::string str, char sep)
{
	std::vector<std::string>	result;
	std::string				tmp;

	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (*it == sep)
		{
			result.push_back(tmp);
			tmp.clear();
		}
		else
			tmp += *it;
	}
	if (!tmp.empty())
		result.push_back(tmp);
	return (result);
}
