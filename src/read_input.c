/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:19:55 by jlehtone          #+#    #+#             */
/*   Updated: 2024/09/25 11:28:56 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
		print_node(shell->token_pointer);
		if (builtins(shell) == 1) // for testing purposes
		{
			//free_and_exit();
		}
		//if (tokenize_input(shell) == true)
		// 	shell->exit_code = execute(shell->token_pointer); // MAKE LATER
		add_history(shell->user_input);
	}
	free(shell->user_input); // replace with custom free function?
	return (shell->exit_code);
}
