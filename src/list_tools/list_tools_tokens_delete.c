/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools_tokens_delete.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 15:59:02 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/13 11:45:03 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// The function deletes the token provided
void	delete_one_token(t_token *lst)
{
	if (!lst || !lst->line)
		return ;
	free(lst->line);
	free(lst);
}

// The function deletes all the tokens in the linked list
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
