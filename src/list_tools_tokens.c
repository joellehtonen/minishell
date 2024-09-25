/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 15:13:14 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/25 14:29:36 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_node(t_token *lst)
{
	if (lst == NULL)
	{
		printf("The list is empty\n");
		return ;
	}
	while (lst != NULL)
	{
		printf("Token: %s, number: %d\n", lst->line, lst->token_number);
		lst = lst->next;
	}
}

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
	new->if_argument = 0;
	new->if_command = 0;
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

void	ft_lstdelone(t_token *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->line);
	free(lst);
}

void	ft_lstclear(t_token **lst, void (*del)(void *))
{
	t_token	*temp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = temp;
	}
	*lst = NULL;
}
