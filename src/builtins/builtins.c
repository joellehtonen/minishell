//42 header

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
		return(export_exec(&shell->envp_copy, builtins, loop_count));
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

int	export_exec(t_envp **envp_copy, t_token *export, int loop_count)
{
	t_envp	*new;
	t_envp	*temp;
	t_token	*arg;
	
	temp = *envp_copy;
	arg = find_token(export, loop_count, ARG);
	if (!arg)
	{
		while (temp)
		{
			printf("declare -x %s\n", temp->line);
			temp = temp->next;
		}
		return (0);
	}
	if (ft_strchr(arg->line, '=') == NULL)
	{
		//do we need to handle?
		//e.g. if in previous child process variable is assigned
		//free_and_exit(...);
		return (0);
	}
	envp_remove_if_export(&temp, arg->line, ft_strncmp);
	new = ft_lstnew_envp(arg->line);
	if (new == NULL)
	{
		//free_and_exit(...);
		return(1);
	}
	ft_lstadd_back_envp(&temp, new);
	return (0);
}
