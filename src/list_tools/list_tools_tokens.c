/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 15:13:14 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/08 14:48:54 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*find_token(t_token *token, int loop_count, int token_type)
{
	t_token	*temp;

	temp = token;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == token_type)
			return (temp);
		temp = temp->next;
	}
	if (!temp || temp->level != loop_count)
		return ((void *) 0);
	return ((void *) 0);
}

t_token	*find_token_line(t_token *token, int loop, int token_type, char *line)
{
	t_token	*temp;

	temp = token;
	while (temp && temp->level != loop)
		temp = temp->next;
	while (temp && temp->level == loop)
	{
		if (temp->type == token_type && \
			ft_strncmp(temp->line, line, ft_strlen(line) + 1) == 0)
			return (temp);
		temp = temp->next;
	}
	if (!temp || temp->level != loop)
		return ((void *) 0);
	return ((void *) 0);
}

t_token	*find_token_index(t_token *token, int loop_count, \
	int token_type, int error_node)
{
	t_token	*temp;
	t_token	*res;
	int		i;

	i = 0;
	res = NULL;
	temp = token;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->next && temp->level == loop_count && i < error_node)
	{
		if (temp->type == token_type)
			res = temp;
		temp = temp->next;
		i++;
	}
	return (res);
}
