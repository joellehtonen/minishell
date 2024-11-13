/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:41:35 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/12 15:32:18 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	copy_envp(t_envp **envp_copy, char *envp[])
{
	int		i;
	t_envp	*new;

	if (envp == NULL)
		return (2);
	i = 0;
	*envp_copy = NULL;
	while (envp[i])
	{
		new = ft_lstnew_envp(envp[i]);
		if (new == NULL)
		{
			delete_envp_part(envp_copy, i - 1);
			return (1);
		}
		ft_lstadd_back_envp(envp_copy, new);
		i++;
	}
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
			*home = ft_substr(envp_copy->line, 5, \
				ft_strlen(envp_copy->line + 5));
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
			*uname = ft_substr(envp_copy->line, 9, \
				ft_strlen(envp_copy->line + 9));
			if (*uname == NULL)
				return (1);
			return (0);
		}
		if (ft_strncmp(envp_copy->line, "USER=", 5) == 0)
		{
			*uname = ft_substr(envp_copy->line, 5, \
				ft_strlen(envp_copy->line + 5));
			if (*uname == NULL)
				return (1);
			return (0);
		}
		envp_copy = envp_copy->next;
	}
	*uname = ft_strdup("");
	if (!*uname)
		return (1);
	return (0);
}

t_envp	*find_envp_line_simple(t_envp *envp, char *line)
{
	t_envp	*temp;

	temp = envp;
	while (temp)
	{
		if (ft_strncmp(temp->line, line, ft_strlen(line)) == 0)
			return (temp);
		temp = temp->next;
	}
	return ((void *) 0);
}