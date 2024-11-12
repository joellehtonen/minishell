/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:52:56 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/12 11:45:43 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exit_exec(t_shell *shell, t_token *token)
{
	int	exit_code;
	int	index;

	index = 0;
	exit_code = 0;
	if (token->next)
	{
		if (token->next->next && token->next->next->type != PIPE)
			error_printer(shell, "", TOO_MANY_ARGS, true);
		while (token->next->line[index] != '\0')
		{
			if (ft_isdigit(token->next->line[index]) == false
				&& token->next->line[index] != '+'
				&& token->next->line[index] != '-')
				error_printer(shell, "", NUMERIC_ERROR, true);
			index++;
		}
		exit_code = ft_atoi(token->next->line);
		if (exit_code > 255)
			exit_code = exit_code % 256;
		shell->exit_code = exit_code;
	}
	free_and_exit(shell, exit_code);
	return (0);
}
