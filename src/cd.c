/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 08:37:45 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/25 15:02:04 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*no_white_spaces(char *str);

static int	count_w_sp(char *str);

int	cd_exec(t_shell *shell)
{
	char	*old_path;
	char	*new_path;
	int		len;
	
	if (too_many_arg_cd(shell->user_input) == 1)
	{
		printing("cd", ": too many arguments\n", "", 2);
		//free_and_exit();
		//exit(1); // can add when in child process
		return (1);
	}
	
	shell->user_input = no_white_spaces(shell->user_input);
	
	if (ft_strlen(shell->user_input) == 0)
	{
		if (shell->home == NULL)
		{
			printing("cd", ": HOME not set\n", "", 2);
			//free_and_exit();
			//exit(1); // can add when in child process
			return (1);
		}
		if (is_directory(shell->home) == 1)
		{
			printing("cd: ", shell->home, ": No such file or directory\n", 2);
			//exit(1);
			return (0);
		}
		if (chdir(shell->home) == -1)
		{
			perror("chdir() error");
			//free_and_exit();
		}
		return (0);
	}

	if (*(shell->user_input) == '/')
	{
		printing("cd: ", shell->user_input, ": No such file or directory\n", 2);
		//free_and_exit();
		//exit(127); // can add when in child process
		return (1);
	}

	if (ft_strncmp(shell->user_input, "~", 1) != 0 && is_directory(shell->user_input) == 1)
	{
		printing("cd: ", shell->user_input, ": Not a directory\n", 2);
		//free_and_exit();
		//exit(1); // can add when in child process
		return (1);
	}
	
	if (ft_strncmp(shell->user_input, "~", 1) == 0)
	{
		if (shell->home == NULL)
		{
			printing("cd", ": HOME not set\n", "", 2);
			//free_and_exit();
			//exit(1); // can add when in child process
			return (1);
		}
		len = ft_strlen(shell->home) + ft_strlen(shell->user_input + 1) + 2;
		new_path = (char *)malloc(len * sizeof(char));
		if (new_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
		new_path = ft_strjoin(shell->home, shell->user_input + 1);
		if (new_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
	}
	else
	{
		old_path = (char *)malloc(BUFF_SIZE * sizeof(char));
		if (old_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
		if (getcwd(old_path, BUFF_SIZE) == NULL)
		{
			perror("getcwd error");
			//free_and_exit();
		}
		len = ft_strlen(old_path) + ft_strlen(shell->user_input) + 2;
		new_path = (char *)malloc(len * sizeof(char));
		if (new_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
		new_path = ft_strjoin_four(old_path, "/",shell->user_input, "");
		if (new_path == NULL)
		{
			perror("malloc error");
			//free_and_exit();
		}
		free(old_path);
	}
	
	if (access(new_path, F_OK) == -1 && errno == ENOENT)
	{
		if (*shell->user_input == '~')
			printing("cd: ", new_path, ": No such file or directory\n", 2);
		else
			printing("cd: ", shell->user_input, ": No such file or directory\n", 2);
		free(new_path);
		//free_and_exit();
		//exit(1); // can add when in child process
		return (1);
	}
	
	if (chdir(new_path) == -1)
	{
    	perror("chdir() error");
		//free_and_exit();
	}
	free(new_path);
	return (0);
}

static char	*no_white_spaces(char *str)
{
	char	*new_str;
	int		new_len;

	if (ft_strlen(str) == 2)
		str = str + 2;
	else
		str = str + 3;
	new_len = ft_strlen(str) - count_w_sp(str);
	new_str = (char *)malloc((new_len + 1) * sizeof(char));
	while (*str == ' ')
		str++;
	while(*str)
	{
		if ((*str == ' ' && only_spaces(str) == 0) ||\
		*str == '\\' || *str == '\"' || *str == '\'')
			str++;
		else
		{
			*new_str = *str;
			str++;
			new_str++;
		}
	}
	*new_str = '\0';
	new_str = new_str - new_len;
	return (new_str);
}

static int	count_w_sp(char *str)
{
	int	count;

	count = 0;
	while (*str == ' ')
	{
		count++;
		str++;
	}
	while(*str)
	{
		if (*str == ' ' && only_spaces(str) == 0)
			count++;
		if (*str == '\\' || *str == '\"' || *str == '\'')
			count++;
		str++;
	}
	return (count);
}
