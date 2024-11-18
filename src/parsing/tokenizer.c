/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 13:19:30 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/18 16:31:22 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	increment_io(t_shell *shell, int end);
static int	handle_argument(t_shell *shell, int end);
static int	create_new_token(t_shell *shell, int end, int start, int token_num);

// cuts the user input into smaller tokens for easier handling
void	tokenize_input(t_shell *shell)
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

// if tokenizer meets I/O operator, increments the index
static int	increment_io(t_shell *shell, int end)
{
	end++;
	if (shell->user_input[end] == shell->user_input[end - 1])
		end++;
	return (end);
}

// finds an end to the token by looking for non-quoted spaces/IO-operators
// to do so, flips quote variables in shell as it encounters them
static int	handle_argument(t_shell *shell, int end)
{
	reset_quotes(shell);
	while (shell->user_input[end] != '\0')
	{
		if (is_quote(shell->user_input[end]) == S_QUOTE)
			shell->single_quote = !shell->single_quote;
		else if (is_quote(shell->user_input[end]) == D_QUOTE)
			shell->double_quote = !shell->double_quote;
		if ((is_io(shell->user_input[end]) != false
				|| is_space(shell->user_input[end]) == true)
			&& (shell->single_quote == false && shell->double_quote == false))
		{
			break ;
		}
		end++;
	}
	return (end);
}

// creates a new token to the end of the list, 
// based on start and end indexes
static int	create_new_token(t_shell *shell, int end, int start, int token_num)
{
	t_token	*new;
	int		token_index;

	token_index = 0;
	new = ft_lstnew_token(NULL);
	if (new == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
	new->line = malloc(sizeof(char) * (end - start + 1));
	if (new->line == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
	while (start != end)
		new->line[token_index++] = shell->user_input[start++];
	new->line[token_index] = '\0';
	new->token_number = token_num;
	new->expanded = false;
	ft_lstadd_back_token(&shell->token_pointer, new);
	return (start);
}
