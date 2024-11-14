/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:54:06 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/14 14:25:23 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// The function unsets selected variables in the minishell environment
int	unset_exec(t_envp **envp_copy, t_token *unset)
{
	t_token	*temp;

	temp = unset->next;
	while (temp && temp->type == ARG)
	{
		envp_remove_if_line(envp_copy, temp->line, ft_strncmp);
		temp = temp->next;
	}
	return (0);
}
