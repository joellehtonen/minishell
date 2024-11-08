/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 10:34:20 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/08 10:40:46 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	env_exec(t_envp *envp_copy, t_shell *shell)
{
	while (envp_copy)
	{
		printf("%s\n", envp_copy->line);
		envp_copy = envp_copy->next;
	}
	free_and_exit(shell, 0);
	return (0);
}

int	unset_exec(t_envp **envp_copy, t_token *unset, int loop_count)
{
	t_token	*arg;

	arg = find_token(unset, loop_count, ARG);
	if (!arg)
		return (0);
	envp_remove_if_line(envp_copy, arg->line, ft_strncmp);
	return (0);
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
