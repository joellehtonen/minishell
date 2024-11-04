/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:01:51 by aklimchu          #+#    #+#             */
/*   Updated: 2024/10/31 11:02:17 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int pwd_exec();

int	exec_builtins(t_shell *shell, int loop_count)
{
	t_token	*builtins;

	if (find_token_line(shell->token_pointer, loop_count, COMM, "cd"))
	{
		builtins = find_token_line(shell->token_pointer, loop_count, COMM, "cd");
		return(cd_exec(shell, builtins, loop_count));
	}
	if (find_token_line(shell->token_pointer, loop_count, COMM, "exit"))
	{
		builtins = find_token_line(shell->token_pointer, loop_count, COMM, "exit");
		return(exit_exec(shell, builtins));
	}
	if (find_token_line(shell->token_pointer, loop_count, COMM, "env"))
		return(env_exec(shell->envp_copy, shell));
	if (find_token_line(shell->token_pointer, loop_count, COMM, "export"))
	{
		builtins = find_token_line(shell->token_pointer, loop_count, COMM, "export");
		return(export_exec(&shell->envp_copy, builtins, loop_count, shell));
	}
	if (find_token_line(shell->token_pointer, loop_count, COMM, "unset"))
	{
		builtins = find_token_line(shell->token_pointer, loop_count, COMM, "unset");
		return(unset_exec(&shell->envp_copy, builtins, loop_count));
	}
	if (find_token_line(shell->token_pointer, loop_count, COMM, "echo"))
	{
		builtins = find_token_line(shell->token_pointer, loop_count, COMM, "echo");
		return(echo(shell, builtins));
	}
	if (find_token_line(shell->token_pointer, loop_count, COMM, "pwd"))
		return(pwd_exec());
	return(0);
}

static int pwd_exec()
{
	char	*pwd;
	
	pwd = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (pwd == NULL)
	{
		perror("malloc error");
		//free_and_exit();
	}
	if (getcwd(pwd, BUFF_SIZE) == NULL)
	{
		perror("getcwd error");
		//free_and_exit();
	}
	printf("%s\n", pwd);
	free(pwd);
	return(0);
}
