/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:55:24 by jlehtone          #+#    #+#             */
/*   Updated: 2024/10/28 14:13:25 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	count_IO(t_shell *shell, int index)
{
	int	count;
	count = 0;
	while (isIO(shell->user_input[index]) != false)
	{
		count++;
		index++;
	}
	return (count);
}

static int	check_consecutive_IO(t_shell *shell, int index)
{
	int	count;

	count = 0;
	if (isIO(shell->user_input[index]) != false && isIO(shell->user_input[index + 1]) == false)
		return (SUCCESS);
	if (isIO(shell->user_input[index]) == PIPE)
	{
		index++;
		if (isIO(shell->user_input[index]) == PIPE)
		{
			index++;
			if (isIO(shell->user_input[index]) == PIPE)
				return (FAILURE);
		}
		if (isIO(shell->user_input[index]) == false)
			return (SUCCESS);
	}
	if (isIO(shell->user_input[index]) == REDIR)
	{
		count = count_IO(shell, index);
		if (is_valid_redir(shell, index, index + 1) == true
			&& count <= 2)
			return (SUCCESS);
	}
	return (FAILURE);
}

static int	check_redir_location(t_shell *shell, int index)
{
	index++;
	while (shell->user_input[index] != '\0')
	{
		if (ft_isspace(shell->user_input[index]) == true)
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

static int	check_pipe_location(t_shell *shell, int old_index)
{
	int new_index;

	new_index = old_index + 1;
	while (shell->user_input[new_index] != '\0')
	{
		if (ft_isspace(shell->user_input[new_index]) == true)
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
		if (ft_isspace(shell->user_input[new_index]) == true)
			new_index--;
		else
			return (SUCCESS);
	}
	return (FAILURE);
}

static int	validate_IO(t_shell *shell, int index)
{
	if (shell->user_input[index] == '|')
	{
		if (check_pipe_location(shell, index) == FAILURE)
		{
			error_printer(shell, PIPE_ERROR, false);
			return (FAILURE);
		}
	}
	if (shell->user_input[index] == '<' || shell->user_input[index] == '>')
	{
		if (check_redir_location(shell, index) == FAILURE)
		{
			error_printer(shell, REDIR_ERROR, false);
			return (FAILURE);
		}
	}
	if (check_consecutive_IO(shell, index) == FAILURE)
		{
			error_printer(shell, SYNTAX_ERROR, false);
			return (FAILURE);
		}
	return (SUCCESS);
}

int input_error_check(t_shell *shell)
{
	int	single_quotes;
	int	double_quotes;
	int	index;
	int	count;
	
	index = 0;
	single_quotes = 0;
	double_quotes = 0;
	if (ft_strlen(shell->user_input) == 0)
		return (FAILURE);
	while (shell->user_input[index] != '\0')
	{
		if (shell->user_input[index] == '\'')
		{
			if (shell->user_input[index - 1] != '\\')
				single_quotes++;
		}
		if (shell->user_input[index] == '\"')
		{
			if (shell->user_input[index - 1] != '\\')
				double_quotes++;
		}
		if (isIO(shell->user_input[index]) != false)
		{
			if (validate_IO(shell, index) == SUCCESS)
			{
				count = count_IO(shell, index);
				index = index + count;
			}
			else
				return (FAILURE);
		}
		else
			index++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
	{
		error_printer(shell, QUOTE_ERROR, false);
		return (FAILURE);
	}
	return (SUCCESS);
}

