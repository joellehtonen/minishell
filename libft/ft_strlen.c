/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 10:30:03 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/13 10:14:12 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// The function implements the same behaviour as strlen from libc.

size_t	ft_strlen(const char *c)
{
	size_t		i;

	i = 0;
	if (!c)
		return (0);
	while (*c)
	{
		i++;
		c++;
	}
	return (i);
}
