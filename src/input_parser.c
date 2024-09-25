/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 13:19:30 by jlehtone          #+#    #+#             */
/*   Updated: 2024/09/25 14:18:52 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int isquote(char character)
{
	if (character == '\'')
		return (1);
	if (character == '\"')
		return (2);
	else
		return (0);
}

void tokenize_input(t_shell *shell)
{
	int		token_index;
	int		start;
	int		end;
	int		quotes_on;
	int		token_number;
	t_token *new;

	start = 0;
	token_number = 0;
	quotes_on = false;
	while (shell->user_input[start] != '\0')
	{
		token_index = 0;
		while (shell->user_input[start] == ' ') // add other white spaces as well, maybe define in the header
			start++;
		end = start;
		while (shell->user_input[end] != '\0' && (shell->user_input[end] != ' ' || quotes_on == true))
		{
			if (isquote(shell->user_input[end]) != 0 && quotes_on == false)
				quotes_on = true;
			if (isquote(shell->user_input[end]) != 0 && quotes_on == true)
				quotes_on = false;
			end++;
		}
		//end--;
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
	}
}
