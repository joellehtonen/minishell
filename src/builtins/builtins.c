//42 header

#include "../../inc/minishell.h"

static int pwd_exec();

static void envp_remove_if_data(t_envp **lst, char *data, int (*cmp)());

int	check_if_builtins(t_shell *shell, int loop_count)
{
	t_token	*builtins;

	if (find_token_line(shell->token_pointer, loop_count, COMM, "cd"))
	{
		builtins = find_token_line(shell->token_pointer, loop_count, COMM, "cd");
		//return(cd_exec(builtins, loop_count));
	}
	if (find_token_line(shell->token_pointer, loop_count, COMM, "exit"))
	{
		builtins = find_token_line(shell->token_pointer, loop_count, COMM, "exit");
		return(exit_exec(shell));
	}
	if (find_token_line(shell->token_pointer, loop_count, COMM, "env"))
	{
		builtins = find_token_line(shell->token_pointer, loop_count, COMM, "env");
		return(env_exec(shell->envp_copy));
	}
	if (find_token_line(shell->token_pointer, loop_count, COMM, "export"))
	{
		builtins = find_token_line(shell->token_pointer, loop_count, COMM, "export");
		//return(export_exec(builtins, loop_count));
	}
	if (find_token_line(shell->token_pointer, loop_count, COMM, "unset"))
	{
		builtins = find_token_line(shell->token_pointer, loop_count, COMM, "unset");
		//return(unset_exec(builtins, loop_count));
	}
	if (find_token_line(shell->token_pointer, loop_count, COMM, "echo"))
	{
		builtins = find_token_line(shell->token_pointer, loop_count, COMM, "echo");
		//return(echo_new(builtins, loop_count));
	}
	if (find_token_line(shell->token_pointer, loop_count, COMM, "pwd"))
	{
		builtins = find_token_line(shell->token_pointer, loop_count, COMM, "pwd");
		return(pwd_exec());
	}
	return (1);
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
	return (0);
}

int	export_exec(t_envp **envp_copy, char *input)
{
	t_envp	*new;
	t_envp	*temp;
	
	temp = *envp_copy;
	if (*(input + 6) == '\0')
	{
		while (temp)
		{
			printf("declare -x %s\n", temp->line);
			temp = temp->next;
		}
		return (0);
	}
	input = input + 7;
	if (ft_strchr(input, '=') == NULL)
	{
		//do we need to handle?
		//e.g. if in previous child process variable is assigned
		return (0);
	}
	envp_remove_if_data(envp_copy, input, ft_strncmp);
	new = ft_lstnew_envp(input);
	if (new == NULL)
	{
		//free_and_exit(...);
		return(1);
	}
	ft_lstadd_back_envp(envp_copy, new);
	return (0);
}

static void envp_remove_if_data(t_envp **lst, char *data, int (*cmp)())
{
	t_envp *temp;
	
	if (lst == NULL || *lst == NULL)
		return;
	temp = *lst;
	if (cmp(temp->line, data, ft_strlen(data) \
			- ft_strlen(ft_strchr(data, '=')) + 1) == 0)
	{
		*lst = temp->next;
		free(temp);
		envp_remove_if_data(lst, data, cmp);
	}
	else
	{
		temp = *lst;
		envp_remove_if_data(&temp->next, data, cmp);
	}
}
