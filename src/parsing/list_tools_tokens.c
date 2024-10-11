/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 15:13:14 by aklimchu          #+#    #+#             */
/*   Updated: 2024/10/11 12:53:24 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_lstadd_back_token(t_token **lst, t_token *new)
{
	t_token	*temp;

	if ((!lst) || (!new))
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		temp = ft_lstlast_token(*lst);
		temp->next = new;
	}
	return ;
}

t_token	*ft_lstnew_token(char *content)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(*new));
	if (new == NULL)
		return ((void *) 0);
	new->line = content;
	new->next = NULL;
	new->type = 0;
	new->type = 0;
	new->level = 0;
	return (new);
}

t_token	*ft_lstlast_token(t_token *lst)
{
	t_token	*temp;

	if (lst == NULL)
		return ((void *) 0);
	temp = lst;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	return (temp);
}

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
	return((void *) 0);
}

t_token	*find_token_line(t_token *token, int loop_count, \
	int token_type, char *line)
{
	t_token	*temp;
	
	temp = token;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == token_type && \
			ft_strncmp(temp->line, line, ft_strlen(line)) == 0)
			return (temp);
		temp = temp->next;
	}
	if (!temp || temp->level != loop_count)
		return ((void *) 0);
	return((void *) 0);
}