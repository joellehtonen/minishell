/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:56:21 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/19 09:08:03 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_io(t_shell *shell, int index)
{
	int	count;

	count = 0;
	while (is_io(shell->user_input[index]) != false)
	{
		count++;
		index++;
	}
	return (count);
}

// checks what I/O operators can follow which operator
int	check_consecutive_io(t_shell *shell, int index)
{
	int	count;

	if (is_io(shell->user_input[index]) != false
		&& is_io(shell->user_input[index + 1]) == false)
		return (SUCCESS);
	if (is_io(shell->user_input[index]) == PIPE)
	{
		index++;
		if (is_io(shell->user_input[index]) == PIPE)
		{
			return (FAILURE);
		}
		if (is_io(shell->user_input[index]) == false)
			return (SUCCESS);
	}
	if (is_io(shell->user_input[index]) == REDIR_INPUT \
		|| is_io(shell->user_input[index]) == REDIR_OUTPUT)
	{
		count = count_io(shell, index);
		if (is_valid_redir(shell, index, index + 1) == true && count <= 2)
			return (SUCCESS);
	}
	return (FAILURE);
}

int	check_redir_location(t_shell *shell, int index)
{
	index++;
	while (shell->user_input[index] != '\0')
	{
		if (is_space(shell->user_input[index]) == true)
		{
			index++;
			if (shell->user_input[index] == '|'
				|| shell->user_input[index] == '<'
				|| shell->user_input[index] == '>')
				return (FAILURE);
		}
		else
			break ;
	}
	if (shell->user_input[index] == '\0')
		return (FAILURE);
	else
		return (SUCCESS);
}

int	check_pipe_location(t_shell *shell, int old_index)
{
	int	new_index;

	new_index = old_index + 1;
	while (shell->user_input[new_index] != '\0')
	{
		if (is_space(shell->user_input[new_index]) == true)
		{
			new_index++;
			if (shell->user_input[new_index] == '|')
				return (FAILURE);
		}
		else
			break ;
	}
	if (shell->user_input[new_index] == '\0')
		return (FAILURE);
	new_index = old_index - 1;
	while (new_index >= 0)
	{
		if (is_space(shell->user_input[new_index]) == true)
			new_index--;
		else
			return (SUCCESS);
	}
	return (FAILURE);
}
