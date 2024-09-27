//42 header

#include "../inc/minishell.h"

static int pwd_exec();

static int echo_exec(t_envp *envp_copy, char *input);

static void envp_remove_if_data(t_envp **lst, char *data, int (*cmp)());

int	builtins(t_shell *shell)
{
	char	*input;
	
	input = shell->user_input;
	if (ft_strncmp(input, "cd ", 3) == 0 ||\
		(ft_strncmp(input, "cd", 2) == 0 && ft_strlen(input) == 2))
		return (cd_exec(shell));
	if (ft_strncmp(input, "exit", 4) == 0 &&\
		(*(input + 4) == ' ' || *(input + 4) == '\0'))
		return (exit_exec(shell));
	if (ft_strncmp(shell->user_input, "env", 3) == 0 &&\
		(*(input + 3) == ' ' || *(input + 3) == '\0'))
		return (env_exec(shell->envp_copy));
	if (ft_strncmp(input, "export", 6) == 0 &&\
		(*(input + 6) == ' ' || *(input + 6) == '\0'))
		return (export_exec(&shell->envp_copy, input));
	if (ft_strncmp(shell->user_input, "unset ", 6) == 0)
		return (unset_exec(&shell->envp_copy, input));
	if (ft_strncmp(input, "echo ", 5) == 0)
		return (echo_exec(shell->envp_copy, input));
	if (ft_strncmp(input, "pwd", 3) == 0 &&\
		(*(input + 3) == ' ' || *(input + 3) == '\0'))
		return (pwd_exec());
	return (0);
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

static int echo_exec(t_envp *envp_copy, char *input)
{
	int		nl_flag;

	input = input + 5; // white spaces handled?
	nl_flag = 0;
	if (ft_strncmp(input, "-n", 2) == 0 && *(input + 2) == ' ')
	{
		nl_flag = 1;
		input = input + 3;
	}
	if (*input == '$' && ft_strlen(input) == 1)
		printf("$");
	else if (*input == '$')
	{
		while (envp_copy)
		{
			if (ft_strncmp(envp_copy->line, input + 1, \
				ft_strlen(input + 1)) == 0)
				printf("%s", envp_copy->line + ft_strlen(input));
			envp_copy = envp_copy->next;
		}
	}
	else
		printf("%s", input);
	if (nl_flag == 0)
		printf("%c", '\n');
	//"echo moi $USERNAME" - add this ability when tokens / parsing is ready
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