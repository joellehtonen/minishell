/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools_envps.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:43:20 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/13 11:18:41 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// The function adds a node to the end of provided linked list
void	ft_lstadd_back_envp(t_envp **lst, t_envp *new)
{
	t_envp	*temp;

	if ((!lst) || (!new))
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		temp = ft_lstlast_envp(*lst);
		temp->next = new;
	}
	return ;
}

// The function creates a new node containing the string provided
t_envp	*ft_lstnew_envp(char *content)
{
	t_envp	*new;

	new = (t_envp *)malloc(sizeof(*new));
	if (new == NULL)
		return ((void *) 0);
	new->line = ft_strdup(content);
	if (new->line == NULL)
		return (NULL);
	new->next = NULL;
	return (new);
}

// The function adds a node to the end of provided linked list
// not using memory allocation for the character string
t_envp	*ft_lstnew_envp_no_strdup(char *content)
{
	t_envp	*new;

	new = (t_envp *)malloc(sizeof(*new));
	if (new == NULL)
		return ((void *) 0);
	new->line = content;
	if (new->line == NULL)
		return (NULL);
	new->next = NULL;
	return (new);
}

// The function locates the last node in the linked list
t_envp	*ft_lstlast_envp(t_envp *lst)
{
	t_envp	*temp;

	if (lst == NULL)
		return ((void *) 0);
	temp = lst;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	return (temp);
}
