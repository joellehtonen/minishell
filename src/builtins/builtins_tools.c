/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:50:01 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/05 12:53:55 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*get_pwd_extra(char *home, t_shell *shell);

char	*get_pwd(char *home, t_shell *shell)
{
	char	*pwd;
	char	*new_pwd;
	int		pwd_move;
	
	pwd = get_pwd_extra(home, shell);
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
	if (pwd_move <= 1)
		return (pwd - pwd_move);
	new_pwd = ft_strjoin("~/", pwd);
	if (new_pwd == NULL)
		error_printer(shell, MALLOC_FAIL, true);
	free(pwd - pwd_move); //this is maybe causing crashes
	return(new_pwd);
}

static char	*get_pwd_extra(char *home, t_shell *shell)
{
	char	*pwd;
	char	*new_pwd;
	
	pwd = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (pwd == NULL)
		error_printer(shell, MALLOC_FAIL, true);
	if (getcwd(pwd, BUFF_SIZE) == NULL)
		error_printer(shell, GETCWD_FAIL, true);
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
			error_printer(shell, MALLOC_FAIL, true);
		return (new_pwd);
	}
	if (home == NULL)
		return (pwd);
	if (ft_strncmp(pwd, home, 1024) == 0)
	{
		free(pwd);
		new_pwd = ft_strdup("~");
		if (new_pwd == NULL)
			error_printer(shell, MALLOC_FAIL, true);
		return (new_pwd);
	}
	return (pwd);
}
