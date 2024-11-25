/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_tools_extra.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:21:27 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/25 14:35:34 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	update_pwd_extra(t_envp **envp_copy, char **pwd, t_shell *shell);
static char	*find_pwd(t_envp *envp_copy, t_shell *shell);

// The function updates PWD variable in the minishell environment
int	update_pwd(t_envp **envp_copy, t_shell *shell)
{
	char	*pwd;

	update_old_pwd(envp_copy, shell);
	envp_remove_if_line(envp_copy, "PWD=", ft_strncmp);
	pwd = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (pwd == NULL)
	{
		error_printer(shell, "", MALLOC_FAIL, true);
		return (1);
	}
	if (getcwd(pwd, BUFF_SIZE) == NULL)
	{
		free_str(&pwd);
		error_printer(shell, "", GETCWD_FAIL, true);
		return (1);
	}
	return (update_pwd_extra(envp_copy, &pwd, shell));
}

// The function updates OLDPWD variable in the minishell environment
int	update_old_pwd(t_envp **envp_copy, t_shell *shell)
{
	t_envp	*new;
	char	*old_pwd;
	char	*export_new;

	envp_remove_if_line(envp_copy, "OLDPWD=", ft_strncmp);
	old_pwd = find_pwd(*envp_copy, shell);
	if (!old_pwd)
		return (1);
	export_new = ft_strjoin("OLDPWD=", old_pwd);
	free_str(&old_pwd);
	if (export_new == NULL)
	{
		error_printer(shell, "", MALLOC_FAIL, true);
		return (1);
	}
	new = ft_lstnew_envp_no_strdup(export_new);
	if (new == NULL)
	{
		error_printer(shell, "", MALLOC_FAIL, true);
		return (1);
	}
	ft_lstadd_back_envp(envp_copy, new);
	return (0);
}

// The function returns the copy of PWD value
// from minishell environment
static char	*find_pwd(t_envp *envp_copy, t_shell *shell)
{
	t_envp	*temp_envp;
	char	*pwd_copy;

	temp_envp = envp_copy;
	while (temp_envp)
	{
		if (ft_strncmp(temp_envp->line, "PWD=", 4) == 0)
		{
			pwd_copy = ft_strdup(temp_envp->line + 4);
			if (pwd_copy == NULL)
			{
				error_printer(shell, "", MALLOC_FAIL, true);
				return (NULL);
			}
			return (pwd_copy);
		}
		temp_envp = temp_envp->next;
	}
	return (NULL);
}

// The function adds a new PWD value to the end of linked list
// containing minishell environment
static int	update_pwd_extra(t_envp **envp_copy, char **pwd, t_shell *shell)
{
	char	*export_new;
	t_envp	*new;

	export_new = ft_strjoin("PWD=", *pwd);
	free_str(pwd);
	if (export_new == NULL)
	{
		error_printer(shell, "", MALLOC_FAIL, true);
		return (1);
	}
	new = ft_lstnew_envp_no_strdup(export_new);
	if (new == NULL)
	{
		error_printer(shell, "", MALLOC_FAIL, true);
		return (1);
	}
	ft_lstadd_back_envp(envp_copy, new);
	return (0);
}

// The function checks if provided path leads to a file
int	is_file(char *path)
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
		return (1);
	}
	else
	{
		close(fd);
		return (0);
	}
}
