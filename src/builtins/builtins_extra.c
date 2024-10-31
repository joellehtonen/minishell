//42 header

#include "../../inc/minishell.h"

int	env_exec(t_envp *envp_copy, t_shell *shell)
{
	while (envp_copy)
	{
		printf("%s\n", envp_copy->line);
		envp_copy = envp_copy->next;
	}
	free_and_exit(shell, 0);
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

int	exit_exec(t_shell *shell)
{
	free_and_exit(shell, 0);
	return (0);
}
