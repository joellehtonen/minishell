/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:35:07 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/12 09:47:34 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
