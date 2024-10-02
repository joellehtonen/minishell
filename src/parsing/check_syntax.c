/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:55:24 by jlehtone          #+#    #+#             */
/*   Updated: 2024/10/02 16:42:56 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_consecutive_IO(t_shell *shell, int index1)
{
	int		index2;
	int		index3;
	char	c2;
	char	c3;
	
	index2 = index1 + 1;
	index3 = index1 + 2;
	c2 = shell->user_input[index2];
	c3 = shell->user_input[index3];
	if (isIO(shell, index1) != false && isIO(shell, index2) == false
		&& isIO(shell, index3) == false)
		return (SUCCESS);
	if (isIO(shell, index1) == PIPE)
	{
		if ((isIO(shell, index2) == PIPE && isIO(shell, index3) == false)
			|| (isIO(shell, index2) == REDIR && isIO(shell, index3) == false)
			|| (isIO(shell, index2) == PIPE && isIO(shell, index3) == REDIR)
			|| is_valid_redir(shell, index2, index3) == true)
			return (SUCCESS);
	}
	if (shell->user_input[index1] == '<')
	{
		if (c2 == '<' && c3 == '<')
			return (SUCCESS);
		if (is_valid_redir(shell, index1, index2))
			return SUCCESS;
	}
	if (shell->user_input[index1] == '>')
	{
		if (c2 == '<')
			return (SUCCESS);
		if (c2 == '>')
		{
			if (c3 == '<')
				return (FAILURE);
			return (SUCCESS);
		}
	}
	// if (shell->user_input[index1] == '<' && shell->user_input[index2] == '<' 
	// 	&& shell->user_input[index3] == '<' && isIO(shell, index3 + 1) == false)
	// 	return (SUCCESS);
	// if (isIO(shell, index1) == REDIR)
	// {
	// 	if (is_valid_redir(shell, index1, index2) == true
	// 		&& (is_valid_redir(shell, index2, index3) || isIO(shell, index3) == false))
	// 		return (SUCCESS);
	// }
	return (FAILURE);
}

static int	check_redir_location(t_shell *shell, int index)
{
	index++;
	while (shell->user_input[index] != '\0')
	{
		if (isspaces(shell, index) == true)
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
		if (isspaces(shell, new_index) == true)
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
		if (isspaces(shell, new_index) == true)
			new_index--;
		else
			return (SUCCESS);
	}
	return (FAILURE);
}

int input_error_check(t_shell *shell)
{
	int	single_quotes;
	int	double_quotes;
	int	index;
	
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
		if (shell->user_input[index] == '|')
		{
			if (check_pipe_location(shell, index) == FAILURE)
			{
				error_printer(shell, PIPE_ERROR, false);
				return (FAILURE);
			}
			if (check_consecutive_IO(shell, index) == FAILURE)
			{
				error_printer(shell, SYNTAX_ERROR, false);
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
			if (check_consecutive_IO(shell, index) == FAILURE)
			{
				error_printer(shell, SYNTAX_ERROR, false);
				return (FAILURE);
			}
		}
		index++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
	{
		error_printer(shell, QUOTE_ERROR, false);
		return (FAILURE);
	}
	return (SUCCESS);
}

