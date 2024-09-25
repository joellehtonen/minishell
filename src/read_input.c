/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:19:55 by jlehtone          #+#    #+#             */
/*   Updated: 2024/09/25 16:32:05 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void input_error_check(t_shell *shell)
{
	int	single_quotes;
	int	double_quotes;
	int	index;
	
	index = 0;
	single_quotes = 0;
	double_quotes = 0;
	if (ft_strlen(shell->user_input) == 0)
		free_and_exit(shell); // do we need an error message?
	while (shell->user_input[index] != '\0')
	{
		if (shell->user_input[index] == '\'') // what if quote is inside a string? can these be condensed into one quote variable?
			single_quotes++;
		if (shell->user_input[index] == '\"')
			double_quotes++;
		// some checks about pipes and redirects?
		index++;
	}
	if (single_quotes % 2 != 0 || double_quotes % 2 != 0)
		free_and_exit(shell); // error message?
}

int read_input(t_shell *shell)
{
	char	*prompt;
	
	shell->pwd = get_pwd(shell->home);
	while (true) 
	{
		free(shell->pwd);
		shell->pwd = get_pwd(shell->home);
		prompt = ft_strjoin_four(shell->uname, ":", shell->pwd, "$ ");
		shell->user_input = readline(prompt);
		if (builtins(shell) == 1) // for testing purposes
		{
			//free_and_exit();
		}
		input_error_check(shell);
		tokenize_input(shell);
		print_node(shell->token_pointer); //for testing
		// 	shell->exit_code = execute(shell->token_pointer); // MAKE LATER
		add_history(shell->user_input);
		delete_all_tokens(&shell->token_pointer);
	}
	free(shell->user_input); // replace with custom free function?
	return (shell->exit_code);
}
