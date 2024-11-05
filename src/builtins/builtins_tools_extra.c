/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_tools_extra.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:21:27 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/05 11:44:08 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*find_pwd(t_envp *envp_copy, t_shell *shell);

int	update_pwd(t_envp **envp_copy, t_shell *shell)
{
	t_envp	*new;
	char	*pwd;
	char	*export_new;

	update_old_pwd(envp_copy, shell);
	envp_remove_if_line(envp_copy, "PWD=", ft_strncmp);
	pwd = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (pwd == NULL)
	{
		error_printer(shell, MALLOC_FAIL, true);
		return (1);
	}
	if (getcwd(pwd, BUFF_SIZE) == NULL)
	{
		error_printer(shell, GETCWD_FAIL, true);
		return (1);
	}
	export_new = ft_strjoin("PWD=", pwd);
	if (export_new == NULL)
	{
		error_printer(shell, MALLOC_FAIL, true);
		return (1);
	}
	new = ft_lstnew_envp_no_strdup(export_new);
	if (new == NULL)
	{
		error_printer(shell, MALLOC_FAIL, true);
		return (1);
	}
	ft_lstadd_back_envp(envp_copy, new);
	return (0);
}

int	update_old_pwd(t_envp **envp_copy, t_shell *shell)
{
	t_envp	*new;
	char 	*old_pwd;
	char	*export_new;

	envp_remove_if_line(envp_copy, "OLDPWD=", ft_strncmp);
	old_pwd = find_pwd(*envp_copy, shell);
	if (!old_pwd)
		return (1);
	export_new = ft_strjoin("OLDPWD=", old_pwd);
	if (export_new == NULL)
	{
		error_printer(shell, MALLOC_FAIL, true);
		return (1);
	}
	new = ft_lstnew_envp_no_strdup(export_new);
	if (new == NULL)
	{
		error_printer(shell, MALLOC_FAIL, true);
		return (1);
	}
	ft_lstadd_back_envp(envp_copy, new);
	return (0);
}

static char	*find_pwd(t_envp *envp_copy, t_shell *shell)
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
				error_printer(shell, MALLOC_FAIL, true);
				return (NULL);
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
