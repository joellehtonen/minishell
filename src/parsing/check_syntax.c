/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kattimaijanen <kattimaijanen@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:55:24 by jlehtone          #+#    #+#             */
/*   Updated: 2024/10/30 16:51:04 by kattimaijan      ###   ########.fr       */
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
	if (isIO(shell->user_input[index]) == REDIR_INPUT \
		|| isIO(shell->user_input[index]) == REDIR_OUTPUT)
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
	if (shell->single_quote == true || shell->double_quote == true)
		return (SUCCESS);
	if (isIO(shell->user_input[index]) == false)
		return (SUCCESS);
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

// static int handle_IO(t_shell *shell, int *index)
// {
// 	// int count; 
	
// 	if (isIO(shell->user_input[*index]) != false)
// 	{
// 		if (validate_IO(shell, *index) == SUCCESS)
// 		{
// 			// count = count_IO(shell, *index);
// 			// *index = *index + count;
// 			return (SUCCESS);
// 		}
// 		else
// 			return (FAILURE);
// 	}
// 	return (SUCCESS);
// }

static void count_quotes(t_shell *shell, int index, int *s_quote, int *d_quote)
{
	if (shell->user_input[index] == '\'')
	{
		if (index == 0 || shell->user_input[index - 1] != '\\')
		{
			if (shell->double_quote == false)
			{
				*s_quote = *s_quote + 1;
				shell->single_quote = !shell->single_quote;
			}
		}
	}
	if (shell->user_input[index] == '\"')
	{
		if (index == 0 ||shell->user_input[index - 1] != '\\')
		{
			if (shell->single_quote == false)
			{
				*d_quote = *d_quote + 1;
				shell->double_quote = !shell->double_quote;
			}
		}
	}
}

int input_error_check(t_shell *shell)
{
	int	single_quotes;
	int	double_quotes;
	int	index;
	
	index = 0;
	single_quotes = 0;
	double_quotes = 0;
	reset_quotes(shell);
	if (ft_strlen(shell->user_input) == 0)
		return (FAILURE);
	while (shell->user_input[index] != '\0')
	{
		count_quotes(shell, index, &single_quotes, &double_quotes);
		if (validate_IO(shell, index) == FAILURE)
			return (FAILURE);
		index++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
	{
		error_printer(shell, QUOTE_ERROR, false);
		return (FAILURE);
	}
	return (SUCCESS);
}

