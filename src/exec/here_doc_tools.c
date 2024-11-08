/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 13:06:14 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/08 13:07:16 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

size_t	ft_strchr_fix(const char *s, int c)
{
	char	new_c;
	int		i;

	new_c = c + '\0';
	i = 0;
	while (1)
	{
		if (*s == new_c)
			return (i);
		s++;
		i++;
	}
	return (0);
}

t_token	*find_here_doc_token(t_token *token)
{
	t_token	*temp;

	temp = token;
	while (temp)
	{
		if (temp->type == REDIR_INPUT && ft_strncmp(temp->line, "<<\0", 3) == 0 \
			&& temp->next && temp->next->type == DELIM)
			return (temp);
		temp = temp->next;
	}
	return ((void *) 0);
}
