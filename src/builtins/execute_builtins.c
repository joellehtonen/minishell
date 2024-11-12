/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:01:51 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/12 10:53:36 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	exec_builtins_extra(t_shell *shell, t_token	*token, int loop);
static int	pwd_exec(t_shell *shell);

int	exec_builtins(t_shell *shell, int loop)
{
	t_token	*token;
	t_token	*builtins;

	token = shell->token_pointer;
	if (find_token_line(token, loop, COMM, "cd"))
	{
		builtins = find_token_line(token, loop, COMM, "cd");
		return (cd_exec(shell, builtins, loop));
	}
	if (find_token_line(token, loop, COMM, "exit"))
	{
		builtins = find_token_line(token, loop, COMM, "exit");
		return (exit_exec(shell, builtins));
	}
	if (find_token_line(token, loop, COMM, "env"))
		return (env_exec(shell->envp_copy, shell, token, loop));
	if (find_token_line(token, loop, COMM, "export"))
	{
		builtins = find_token_line(token, loop, COMM, "export");
		return (export_exec(&shell->envp_copy, builtins, loop, shell));
	}
	return (exec_builtins_extra(shell, token, loop));
}

static int	exec_builtins_extra(t_shell *shell, t_token	*token, int loop)
{
	t_token	*builtins;

	if (find_token_line(token, loop, COMM, "unset"))
	{
		builtins = find_token_line(token, loop, COMM, "unset");
		return (unset_exec(&shell->envp_copy, builtins, loop));
	}
	if (find_token_line(token, loop, COMM, "echo"))
	{
		builtins = find_token_line(token, loop, COMM, "echo");
		return (echo(shell, builtins));
	}
	if (find_token_line(token, loop, COMM, "pwd"))
		return (pwd_exec(shell));
	return (0);
}

static int	pwd_exec(t_shell *shell)
{
	char	*pwd;

	pwd = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (pwd == NULL)
	{
		error_printer(shell, "", MALLOC_FAIL, true);
		return (1);
	}
	if (getcwd(pwd, BUFF_SIZE) == NULL)
	{
		printf("%s\n", shell->pwd);
		free_str(&pwd);
		//error_printer(shell, "", GETCWD_FAIL, true);
		return (0);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
