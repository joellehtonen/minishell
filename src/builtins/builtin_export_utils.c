/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:35:07 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/19 10:35:44 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// The function checks the string for validity
// (related to "export" function)
int	check_str(char *str, int minus, int plus, int equal)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '-')
			minus = i;
		if (str[i] == '+')
			plus = i;
		if (str[i] == '=')
			equal = i;
		if (minus > -1 && (equal == -1 || \
			minus < equal))
			return (1);
		if (plus > -1 && \
			((equal == -1 && str[i + 1] != '=') || \
			plus + 1 < equal))
			return (1);
		i++;
	}
	return (0);
}

// The function checks if the string is empty 
// or if it containts only numerical characters
int	only_digits_or_empty(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] >= 48 && str[i] <= 57)
			count++;
		i++;
	}
	if (i == count)
		return (1);
	return (0);
}

int	forbidden_symbols(char *str)
{
	int	last_equal;
	int	i;

	last_equal = 0;
	i = 0;
	while (str[i])
	{
		if (str[i++] == '=')
			last_equal = i - 1;
	}
	i = 0;
	while(i < last_equal)
	{
		if (str[i] == 43 && i != last_equal - 1)
			return (1);
		if (str[i] < 43 || (str[i] > 43 && str[i] < 48) || \
			(str[i] > 57 && str[i] < 65) || (str[i] > 90 && str[i] < 95) \
			|| str[i] == 96 || str[i] > 122)
			return (1);
		i++;
	}
	return (0);
}
