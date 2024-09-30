/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools_tokens_delete.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 15:59:02 by jlehtone          #+#    #+#             */
/*   Updated: 2024/09/30 08:41:30 by aklimchu         ###   ########.fr       */
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
		printf("Token #%d: %s, type %d, level %d\n", lst->token_number, lst->line, lst->type, lst->level);
		lst = lst->next;
	}
}

void	delete_one_token(t_token *lst)
{
	if (!lst)
		return ;
	free(lst->line);
	free(lst);
}

void	delete_all_tokens(t_token **lst)
{
	t_token	*temp;

	if (!lst)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		delete_one_token(*lst);
		*lst = temp;
	}
	*lst = NULL;
}
