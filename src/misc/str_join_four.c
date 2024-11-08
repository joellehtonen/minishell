/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_join_four.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:27:13 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/08 14:50:13 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strjoin_four(char *s1, char *s2, char *s3, char *s4)
{
	char	*new_s;
	size_t	totallen;

	totallen = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + ft_strlen(s4);
	new_s = (char *)malloc((totallen + 1) * sizeof(char));
	if (new_s == NULL)
		return ((void *)0);
	while (*s1)
		*(new_s++) = *(s1++);
	while (*s2)
		*(new_s++) = *(s2++);
	while (*s3)
		*(new_s++) = *(s3++);
	while (*s4)
		*(new_s++) = *(s4++);
	*new_s = '\0';
	return (new_s - totallen);
}
