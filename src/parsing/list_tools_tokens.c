/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 15:13:14 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/27 15:33:32 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
