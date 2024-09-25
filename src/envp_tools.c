/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:43:33 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/25 14:45:42 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	add_envp(t_envp **envp_copy);

int copy_envp(t_shell *shell, t_envp **envp_copy, char *envp[])
{
	int		i;
	t_envp	*new;

	(void)shell;
	if (envp == NULL || envp[0] == NULL)
		return (add_envp(envp_copy));
	i = 0;
	*envp_copy = NULL;
	while (envp[i])
	{
		new = ft_lstnew_envp(envp[i]);
		if (new == NULL)
		{
			//free_lists(...);
			return(1);
		}
		ft_lstadd_back_envp(envp_copy, new);
		i++;
	}
	if (copy_path(&shell->path, *envp_copy) == 1)
		return (1);
	if (copy_home(&shell->home, *envp_copy) == 1)
		return (1);
	return (copy_uname(&shell->uname, *envp_copy));
}

static int	add_envp(t_envp **envp_copy)
{
	t_envp	*new;
	
	*envp_copy = NULL;
	new = ft_lstnew_envp(getenv("HOME"));
	if (new == NULL)
	{
		//free_lists(...);
		return(1);
	}
	ft_lstadd_back_envp(envp_copy, new);
	new = ft_lstnew_envp(getenv("PATH"));
	if (new == NULL)
	{
		//free_lists(...);
		return(1);
	}
	ft_lstadd_back_envp(envp_copy, new);
	new = ft_lstnew_envp(getenv("USER"));
	if (new == NULL)
	{
		//free_lists(...);
		return(1);
	}
	ft_lstadd_back_envp(envp_copy, new);
	return (0);
}

int	copy_path(t_envp **path, t_envp *envp_copy)
{
	while (envp_copy)
	{
		if (ft_strncmp(envp_copy->line, "PATH=", 5) == 0)
			return (ft_split_list(path, envp_copy->line + 5, ':'));
		envp_copy = envp_copy->next;
	}
	*path = NULL;
	return (0);
}

int	copy_home(char **home, t_envp *envp_copy)
{
	while (envp_copy)
	{
		if (ft_strncmp(envp_copy->line, "HOME=", 5) == 0)
		{
			*home = ft_substr(envp_copy->line, 5, ft_strlen(envp_copy->line + 5));
			if (*home == NULL)
				return (1);
			return (0);
		}
		envp_copy = envp_copy->next;
	}
	*home = NULL;
	return (0);
}

int	copy_uname(char **uname, t_envp *envp_copy)
{
	while (envp_copy)
	{
		if (ft_strncmp(envp_copy->line, "USERNAME=", 9) == 0)
		{
			*uname = ft_substr(envp_copy->line, 9, ft_strlen(envp_copy->line + 9));
			if (*uname == NULL)
				return (1);
			return (0);
		}
		if (ft_strncmp(envp_copy->line, "USER=", 5) == 0)
		{
			*uname = ft_substr(envp_copy->line, 5, ft_strlen(envp_copy->line + 5));
			if (*uname == NULL)
				return (1);
			return (0);
		}
		envp_copy = envp_copy->next;
	}
	*uname = NULL;
	return (0);
}
