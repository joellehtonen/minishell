/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory_extra.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:15:27 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/14 14:11:34 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	delete_one_envp_node(t_envp *node);

// The function deletes the linked list containing minishell environment
void	delete_envp(t_envp **copy_envp)
{
	t_envp	*temp;

	if (!(*copy_envp))
		return ;
	while (*copy_envp)
	{
		temp = (*copy_envp)->next;
		delete_one_envp_node(*copy_envp);
		*copy_envp = temp;
	}
	*copy_envp = NULL;
}

// The function deletes a node provided
static void	delete_one_envp_node(t_envp *node)
{
	if (!node || !node->line)
		return ;
	free(node->line);
	free(node);
}

// The function deletes the linked list 
// containing the known amount of nodes
void	delete_envp_part(t_envp **copy_envp, int count)
{
	t_envp	*temp;
	int		i;

	if (!(*copy_envp))
		return ;
	i = 0;
	while (*copy_envp && i <= count)
	{
		temp = (*copy_envp)->next;
		delete_one_envp_node(*copy_envp);
		*copy_envp = temp;
		i++;
	}
	*copy_envp = NULL;
}
