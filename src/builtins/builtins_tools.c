/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:50:01 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/11 12:51:46 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*current_location(t_shell *shell);
static char	*get_pwd_extra(char *home, t_shell *shell);
static char	*pwd_home(char **pwd, char *home, t_shell *shell);
static char	*join_pwd(char *pwd, int pwd_move, t_shell *shell);

char	*get_pwd(char *home, t_shell *shell)
{
	char	*pwd;
	int		pwd_move;

	if (!home)
		return (current_location(shell));
	pwd = get_pwd_extra(home, shell);
	if (!pwd)
		return (current_location(shell));
	pwd_move = 0;
	while ((*home) && (*pwd) && *pwd == *home)
	{
		pwd++;
		home++;
		pwd_move++;
	}
	if (*pwd)
	{
		pwd++;
		pwd_move++;
	}
	return (join_pwd(pwd, pwd_move, shell));
}

static char	*get_pwd_extra(char *home, t_shell *shell)
{
	char	*pwd;
	char	*new_pwd;

	pwd = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (pwd == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
	if (getcwd(pwd, BUFF_SIZE) == NULL)
		error_printer(shell, "", GETCWD_FAIL, true);
	if (ft_strncmp(pwd, "/", 1) == 0 && ft_strlen(pwd) == 1)
	{
		free(pwd);
		new_pwd = ft_strdup("/");
		return (new_pwd);
	}
	if (ft_strncmp(pwd, "/home", 5) == 0 && ft_strlen(pwd) == 5)
	{
		free(pwd);
		new_pwd = ft_strdup("/home");
		if (new_pwd == NULL)
			error_printer(shell, "", MALLOC_FAIL, true);
		return (new_pwd);
	}
	return (pwd_home(&pwd, home, shell));
}

static char	*pwd_home(char **pwd, char *home, t_shell *shell)
{
	char	*new_pwd;

	if (home == NULL)
		return (*pwd);
	if (ft_strncmp(*pwd, home, 1024) == 0)
	{
		free(*pwd);
		new_pwd = ft_strdup("~");
		if (new_pwd == NULL)
			error_printer(shell, "", MALLOC_FAIL, true);
		return (new_pwd);
	}
	return (*pwd);
}

static char	*join_pwd(char *pwd, int pwd_move, t_shell *shell)
{
	char	*new_pwd;

	if (pwd_move <= 1)
		return (pwd - pwd_move);
	new_pwd = ft_strjoin("~/", pwd);
	if (new_pwd == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
	free(pwd - pwd_move);
	return (new_pwd);
}

static char	*current_location(t_shell *shell)
{
	char	*pwd;

	pwd = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (pwd == NULL)
	{
		error_printer(shell, "", MALLOC_FAIL, true);
		return (NULL);
	}
	if (getcwd(pwd, BUFF_SIZE) == NULL)
	{
		error_printer(shell, "", GETCWD_FAIL, true);
		return (NULL);
	}
	return (pwd);
}
