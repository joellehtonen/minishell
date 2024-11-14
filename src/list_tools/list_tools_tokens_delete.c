/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_tools_tokens_delete.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 15:59:02 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/14 14:09:56 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	delete_one_token(t_token *lst)
{
	if (!lst || !lst->line)
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
