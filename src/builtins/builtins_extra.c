//42 header

#include "../../inc/minishell.h"

int	env_exec(t_envp *envp_copy)
{
	while (envp_copy)
	{
		printf("%s\n", envp_copy->line);
		envp_copy = envp_copy->next;
	}
	//free_and_exit();
	return (0);
}

int	unset_exec(t_envp **envp_copy, t_token *unset, int loop_count)
{
	t_token	*arg;
	
	arg = find_token(unset, loop_count, ARG);
	if (!arg)
		return (0);
	envp_remove_if_line(envp_copy, arg->line, ft_strncmp);
	return (0);
}

int	exit_exec(t_shell *shell, t_token *token)
{
	int exit_code;
	int index;

	index = 0;
	if (token->next)
	{
		if (token->next->next)
			error_printer(shell, TOO_MANY_ERROR, true);
		while (token->next->line[index] != '\0')
		{
			if (ft_isdigit(token->next->line[index]) == false
				&& token->next->line[index] != '+'
				&& token->next->line[index] != '-')
				error_printer(shell, NUMERIC_ERROR, true);
			index++;
		}
		exit_code = ft_atoi(token->next->line);
		shell->exit_code = exit_code;
	}
	free_and_exit(shell, exit_code);
	return (0);
}
