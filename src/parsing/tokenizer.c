/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 13:19:30 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/05 15:08:48 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	create_new_token(t_shell *shell, int end, int start, int token_number)
{
	t_token *new;
	int		token_index;
	
	token_index = 0;
	new = ft_lstnew_token(NULL);
	new->line = malloc(sizeof(char) * (end - start + 1));
	if (new->line == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
	while (start != end)
		new->line[token_index++] = shell->user_input[start++];
	new->line[token_index] = '\0';
	new->token_number = token_number;
	new->expanded = false;
	ft_lstadd_back_token(&shell->token_pointer, new);
	return (start);
}

static int handle_argument(t_shell *shell, int end)
{
	int		IO_met;

	reset_quotes(shell);
	IO_met = false;
	while (shell->user_input[end] != '\0' 
			&& (is_space(shell->user_input[end]) == false 
			|| shell->single_quote == true || shell->double_quote == true))
	{
		if (is_quote(shell->user_input[end]) == S_QUOTE)
			shell->single_quote = !shell->single_quote;
		else if (is_quote(shell->user_input[end]) == D_QUOTE)
			shell->double_quote = !shell->double_quote;
		if (is_io(shell->user_input[end]) != false 
			&& (shell->single_quote == false && shell->double_quote == false))
			{
				if (IO_met == true)
					end++;
				IO_met = !IO_met;
				break ;
			}
		end++;
	}
	return (end);
}

static int increment_io(t_shell *shell, int end)
{
	end++;
	if (shell->user_input[end] == shell->user_input[end - 1])
		end++;
	return (end);
}

void tokenize_input(t_shell *shell)
{
	int		start;
	int		end;
	int		token_number;

	start = 0;
	token_number = 0;
	while (shell->user_input[start] != '\0')
	{
		while (is_space(shell->user_input[start]) == true)
			start++;
		if (shell->user_input[start] == '\0')
			break ;
		end = start;
		if (is_io(shell->user_input[end]) != false)
			end = increment_io(shell, end);
		else
			end = handle_argument(shell, end);
		start = create_new_token(shell, end, start, token_number);
		token_number++;
	}
}
