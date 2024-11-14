/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 13:06:14 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/14 14:10:48 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// The function returns the index number of a given character
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

// The function locates the token containing here_doc
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
