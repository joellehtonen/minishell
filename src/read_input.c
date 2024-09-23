/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:19:55 by jlehtone          #+#    #+#             */
/*   Updated: 2024/09/23 16:13:54 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int read_input(t_shell *shell)
{
	while (true) 
	{
		shell->user_input = readline("Our_shell: ");
		//if (tokenize_input(shell) == true)
		// 	shell->exit_code = execute(shell->token_pointer); // MAKE LATER
		add_history(shell->user_input);
	}
	free(shell->user_input); // replace with custom free function?
	return (shell->exit_code);
}
