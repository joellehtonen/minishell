/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 13:19:30 by jlehtone          #+#    #+#             */
/*   Updated: 2024/09/27 09:52:16 by jlehtone         ###   ########.fr       */
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
		return (1);
	if (c == '\"')
		return (2);
	else
		return (false);
}

void tokenize_input(t_shell *shell)
{
	int		token_index;
	int		start;
	int		end;
	int		quotes_on;
	int		token_number;
	int		sep_met;
	t_token *new;

	start = 0;
	token_number = 0;
	quotes_on = false;
	sep_met = false;
	while (shell->user_input[start] != '\0')
	{
		token_index = 0;
		while (isseparator(shell->user_input[start]) == SPACES) //should we skip other separators as well?
			start++;
		end = start;
		//while (shell->user_input[end] != '\0' && (isseparator(shell->user_input[end]) == false || quotes_on == true))
		while (shell->user_input[end] != '\0' && (shell->user_input[end] != ' ' || quotes_on == true))
		{
			if (isquote(shell->user_input[end]) != false)
				quotes_on = !quotes_on;
			if (isseparator(shell->user_input[end]) != false && quotes_on == false)
			{
				if (sep_met == true)
					end++;
				sep_met = !sep_met;
				break ;
			}
			end++;
		}
		new = ft_lstnew_token(NULL);
		new->line = malloc(sizeof(char) * (end - start + 1));
		if (new->line == NULL)
			free_and_exit(shell); // DO THIS LATER
		while (start != end)
			new->line[token_index++] = shell->user_input[start++];
		new->line[token_index] = '\0';
		new->token_number = token_number;
		token_number++;
		ft_lstadd_back_token(&shell->token_pointer, new);
		// new->level++;
		//print_node(shell->token_pointer); //for testing
	}
}