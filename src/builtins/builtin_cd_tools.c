/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 10:57:09 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/25 14:34:48 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*handle_home(t_shell *shell, t_token *arg);
static char	*new_pwd(t_token *arg, t_shell *shell);
static char	*get_old_pwd(t_envp *envp_copy, t_shell *shell);

// The function returns a new path based on the argument provided
char	*get_new_path(t_shell *shell, t_token *arg)
{
	char	*old_pwd;
	char	*new_root;

	if (ft_strncmp(arg->line, "-\0", 2) == 0)
	{
		old_pwd = get_old_pwd(shell->envp_copy, shell);
		if (old_pwd)
			printf("%s\n", old_pwd);
		return (old_pwd);
	}
	if (ft_strncmp(arg->line, "~", 1) == 0)
		return (handle_home(shell, arg));
	if (ft_strncmp(arg->line, "/", 1) == 0)
	{
		new_root = ft_strdup(arg->line);
		if (!new_root)
			error_printer(shell, "", MALLOC_FAIL, true);
		return (new_root);
	}
	else
		return (new_pwd(arg, shell));
}

// The function expands the symbol to HOME environment variable
static char	*handle_home(t_shell *shell, t_token *arg)
{
	char	*new_path;

	if (shell->home == NULL)
	{
		error_printer(shell, "", "cd: HOME not set\n", true);
		return (NULL);
	}
	new_path = ft_strjoin(shell->home, arg->line + 1);
	if (new_path == NULL)
	{
		error_printer(shell, "", MALLOC_FAIL, true);
		return (NULL);
	}
	return (new_path);
}

// The function creates a new path based on current path and user input
static char	*new_pwd(t_token *arg, t_shell *shell)
{
	char	*old_path;
	char	*new_path;

	old_path = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (old_path == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
	if (getcwd(old_path, BUFF_SIZE) == NULL)
	{
		error_printer(shell, "", GETCWD_FAIL, true);
		free(old_path);
		old_path = ft_strdup("/");
		if (old_path == NULL)
			error_printer(shell, "", MALLOC_FAIL, true);
		return (old_path);
	}
	else
	{
		new_path = ft_strjoin_four(old_path, "/", arg->line, "");
		free_str(&old_path);
		if (new_path == NULL)
			error_printer(shell, "", MALLOC_FAIL, true);
		return (new_path);
	}
}

// The function returns a previous directory (OLDPWD)
static char	*get_old_pwd(t_envp *envp_copy, t_shell *shell)
{
	t_envp	*temp_envp;
	char	*old_pwd_copy;

	temp_envp = envp_copy;
	while (temp_envp)
	{
		if (ft_strncmp(temp_envp->line, "OLDPWD=", 7) == 0)
		{
			old_pwd_copy = ft_strdup(temp_envp->line + 7);
			if (old_pwd_copy == NULL)
				error_printer(shell, "", MALLOC_FAIL, true);
			return (old_pwd_copy);
		}
		temp_envp = temp_envp->next;
	}
	error_printer(shell, "", "cd: OLDPWD not set\n", true);
	return (NULL);
}
