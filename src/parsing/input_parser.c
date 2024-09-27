/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 13:19:30 by jlehtone          #+#    #+#             */
/*   Updated: 2024/09/27 13:54:59 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int isseparator(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
		return (SPACES);
	if (c == '<' || c == '>')
		return (REDIR);
	if (c == '|')
		return (PIPE);
	else
		return (false);
}

int isquote(char c)
{
	if (c == '\'')
		return (SINGLE_QUOTE);
	if (c == '\"')
		return (DOUBLE_QUOTE);
	else
		return (false);
}

static int	create_new_token(t_shell *shell, int end, int start, int token_number)
{
	t_token *new;
	int		token_index;

	token_index = 0;
	new = ft_lstnew_token(NULL);
	new->line = malloc(sizeof(char) * (end - start + 1));
	if (new->line == NULL)
		free_and_exit(shell); // DO THIS LATER
	while (start != end)
		new->line[token_index++] = shell->user_input[start++];
	new->line[token_index] = '\0';
	new->token_number = token_number;
	ft_lstadd_back_token(&shell->token_pointer, new);
	return (start);
}

static int handle_argument(t_shell *shell, int end)
{
	int		quotes_on;
	int		separator_met;

	quotes_on = false;
	separator_met = false;
	while (shell->user_input[end] != '\0' && (shell->user_input[end] != ' ' || quotes_on == true))
	{
		if (isquote(shell->user_input[end]) != false)
			quotes_on = !quotes_on;
		end++;
	}
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
		while (isseparator(shell->user_input[start]) == SPACES)
			start++;
		end = start;
		if (isseparator(shell->user_input[end]) != false)
		{
			end++;
			if (shell->user_input[end] == shell->user_input[end - 1])
				end++;
		}
		else
			end = handle_argument(shell, end);
		start = create_new_token(shell, end, start, token_number);
		token_number++;
		// new->level++;
		//print_node(shell->token_pointer); //for testing
	}
}