/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 12:12:55 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/08 12:43:04 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static size_t	nextc(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (*s != c && (*s))
	{
		s++;
		i++;
	}
	return (i);
}

int	ft_split_list(t_envp **path, char const *s, char c)
{
	t_envp	*new;
	char	*temp;
	int		i;

	*path = NULL;
	i = 0;
	while (*s)
	{
		if (*s == c)
			s++;
		else
		{
			temp = ft_substr(s, 0, nextc(s, c));
			if (temp == NULL)
			{
				delete_envp_part(path, i - 1);
				return (1);
			}
			new = ft_lstnew_envp_no_strdup(temp);
			ft_lstadd_back_envp(path, new);
			s = s + nextc(s, c);
			i++;
		}
	}
	return (0);
}
