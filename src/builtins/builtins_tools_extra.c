/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_tools_extra.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:21:27 by aklimchu          #+#    #+#             */
/*   Updated: 2024/10/30 11:53:36 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*find_pwd(t_envp *envp_copy);

int	update_pwd(t_envp **envp_copy)
{
	t_envp	*new;
	char	*pwd;
	char	*export_new;

	update_old_pwd(envp_copy);
	envp_remove_if_line(envp_copy, "PWD=", ft_strncmp);
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
	export_new = ft_strjoin("PWD=", pwd);
	if (export_new == NULL)
	{
		perror("malloc error");
		//free_and_exit();
	}
	new = ft_lstnew_envp_no_strdup(export_new);
	if (new == NULL)
	{
		//free_and_exit(...);
		return(0);
	}
	ft_lstadd_back_envp(envp_copy, new);
	return (0);
}

int	update_old_pwd(t_envp **envp_copy)
{
	t_envp	*new;
	char 	*old_pwd;
	char	*export_new;

	envp_remove_if_line(envp_copy, "OLDPWD=", ft_strncmp);
	old_pwd = find_pwd(*envp_copy);
	if (!old_pwd)
		return (1);
	export_new = ft_strjoin("OLDPWD=", old_pwd);
	if (export_new == NULL)
	{
		perror("malloc error");
		//free_and_exit();
	}
	new = ft_lstnew_envp_no_strdup(export_new);
	if (new == NULL)
	{
		//free_and_exit(...);
		return(1);
	}
	ft_lstadd_back_envp(envp_copy, new);
	return (0);
}

static char	*find_pwd(t_envp *envp_copy)
{
	t_envp *temp_envp;
	char	*pwd_copy;
	
	temp_envp = envp_copy;
	while (temp_envp)
	{
		if (ft_strncmp(temp_envp->line, "PWD=", 4) == 0)
		{
			pwd_copy = ft_strdup(temp_envp->line + 4);
			if (pwd_copy == NULL)
			{
				perror("malloc error");
				//free_and_exit();
			}
			return (pwd_copy);
		}
		temp_envp = temp_envp->next;
	}
	return (NULL);
}

int	is_directory_new(char *path)
{
	int		fd;
	char	buffer;
	ssize_t	result;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (1);
	result = read(fd, &buffer, 1);
	if (result < 0 && errno == EISDIR)
	{
		close(fd);
		return (0);
	}
	else
	{
		close(fd);
		return (1);
	}
}

int is_file(char *path)
{
	int		fd;
	char	buffer;
	ssize_t	result;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (1);
	result = read(fd, &buffer, 1);
	if (result < 0 && errno == EISDIR)
		return (1);
	else
		return (0);
}