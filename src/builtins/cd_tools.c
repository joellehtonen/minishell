/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 10:57:09 by aklimchu          #+#    #+#             */
/*   Updated: 2024/10/30 12:56:17 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*handle_home(t_shell *shell, t_token *arg);

static char	*new_pwd(t_token *arg);

static  char	*get_old_pwd(t_envp *envp_copy);

char	*get_new_path(t_shell *shell, t_token *arg)
{
	if (ft_strncmp(arg->line, "-\0", 2) == 0)
		return(get_old_pwd(shell->envp_copy));
	if (ft_strncmp(arg->line, "~", 1) == 0)
		return(handle_home(shell, arg));
	if (ft_strncmp(arg->line, "/", 1) == 0)
		return(ft_strdup(arg->line));
	else
		return(new_pwd(arg));
}

static char	*handle_home(t_shell *shell, t_token *arg)
{
	char	*new_path;
	
	if (shell->home == NULL)
	{
		printing("cd", ": HOME not set\n", "", 2);
		//free_and_exit();
		return (NULL);
	}
	new_path = ft_strjoin(shell->home, arg->line + 1);
	if (new_path == NULL)
	{
		perror("malloc error");
		//free_and_exit();
	}
	return (new_path);
}

static char	*new_pwd(t_token *arg)
{
	char	*old_path;
	char	*new_path;
	int		len;
	
	old_path = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (old_path == NULL)
	{
		perror("malloc error");
		//free_and_exit();
		return(NULL);
	}
	if (getcwd(old_path, BUFF_SIZE) == NULL)
	{
		perror("getcwd error");
		//free_and_exit();
		return(NULL);
	}
	len = ft_strlen(old_path) + ft_strlen(arg->line) + 2;
	new_path = (char *)malloc(len * sizeof(char));
	if (new_path == NULL)
	{
		perror("malloc error");
		//free_and_exit();
		return(NULL);
	}
	new_path = ft_strjoin_four(old_path, "/", arg->line, "");
	if (new_path == NULL)
	{
		perror("malloc error");
		//free_and_exit();
	}
	free(old_path);
	return(new_path);
}

static char	*get_old_pwd(t_envp *envp_copy)
{
	t_envp *temp_envp;
	char	*old_pwd_copy;
	
	temp_envp = envp_copy;
	while (temp_envp)
	{
		if (ft_strncmp(temp_envp->line, "OLDPWD=", 7) == 0)
		{
			old_pwd_copy = ft_strdup(temp_envp->line + 7);
			if (old_pwd_copy == NULL)
			{
				perror("malloc error");
				//free_and_exit();
			}
			return (old_pwd_copy);
		}
		temp_envp = temp_envp->next;
	}
	printing("cd: ", "", "OLDPWD not set\n", 2);
	return (NULL);
}
