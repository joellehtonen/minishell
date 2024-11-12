/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:55:34 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/12 13:14:09 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pwd_exec(t_shell *shell)
{
	char	*pwd;

	pwd = (char *)malloc(BUFF_SIZE * sizeof(char));
	if (pwd == NULL)
	{
		error_printer(shell, "", MALLOC_FAIL, true);
		return (1);
	}
	if (getcwd(pwd, BUFF_SIZE) == NULL)
	{
		printf("%s\n", shell->pwd);
		free_str(&pwd);
		return (0);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
