/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools_tokens_extra.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:49:11 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/07 12:49:31 by jlehtone         ###   ########.fr       */
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

int	count_nodes_type(t_token *start, int token_type, int loop_count)
{
	t_token	*temp;
	int		i;

	temp = start;
	i = 0;
	while (temp && temp->level != loop_count)
		temp = temp->next;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == token_type)
			i++;
		temp = temp->next;
	}
	if (!temp || temp->level != loop_count)
		return (i);
	return (i);
}
