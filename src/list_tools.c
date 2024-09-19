/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 15:13:14 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/19 15:19:25 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_lstadd_back_ms(t_envp **lst, t_envp *new)
{
	t_envp	*temp;

	if ((!lst) || (!new))
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		temp = ft_lstlast_ms(*lst);
		temp->next = new;
	}
	return ;
}

t_envp	*ft_lstnew_ms(char *content)
{
	t_envp	*new;

	new = (t_envp *)malloc(sizeof(*new));
	if (new == NULL)
		return ((void *) 0);
	new->line = content;
	new->next = NULL;
	return (new);
}

t_envp	*ft_lstlast_ms(t_envp *lst)
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