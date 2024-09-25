/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:46:57 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/25 15:41:47 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int pwd_exec();

static int echo_exec(char *input);

static int	exit_exec(t_shell *shell);

static int	export_exec(t_envp *envp_copy, char *input);

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
	/*if (ft_strncmp(shell->user_input, "env ", 4) == 0)
		return (env_exec(shell));*/
	if (ft_strncmp(input, "export", 6) == 0 &&\
		(*(input + 6) == ' ' || *(input + 6) == '\0'))
		return (export_exec(shell->envp_copy, input));
	/*if (ft_strncmp(shell->user_input, "unset ", 6) == 0)
		return (unset_exec(shell)); */
	if (ft_strncmp(input, "echo ", 5) == 0)
		return (echo_exec(input));
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

static int echo_exec(char *input)
{
	int		nl_flag;
	
	input = input + 5; // white spaces handled?
	nl_flag = 0;
	if (ft_strncmp(input, "-n", 2) == 0 && *(input + 2) == ' ')
	{
		nl_flag = 1;
		input = input + 3;
	}
	printf("%s", input);
	if (nl_flag == 0)
		printf("%c", '\n');
	return (0);
}

static int	exit_exec(t_shell *shell)
{
	(void)shell;
	//free_and_exit();
	exit(0);
}

static int	export_exec(t_envp *envp_copy, char *input)
{
	if (*(input + 6) == '\0')
	{
		while (envp_copy)
		{
			printf("declare -x %s\n", (envp_copy)->line);
			envp_copy = envp_copy->next;
		}
		return (0);
	}
	input = input + 7;
	if (ft_strchr(input, '=') == NULL)
	{
		//do we need to handle?
	}
	//export to continue...
	/* while (envp_copy)
	{
		if (ft_strncmp(envp_copy->line, "PATH=", 5) == 0)
			return (ft_split_list(path, envp_copy->line + 5, ':'));
		envp_copy = envp_copy->next;
	} */
	return (0);
}