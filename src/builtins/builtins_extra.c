/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 10:34:20 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/12 09:28:38 by jlehtone         ###   ########.fr       */
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

char	*remove_plus(char *data)
{
	char	*new_data;
	int		plus_flag;
	int		data_len;

	data_len = ft_strlen(data);
	new_data = (char *)malloc(data_len * sizeof(char));
	if (!new_data)
		return (NULL);
	plus_flag = false;
	while (*data)
	{
		if (*data == '+' && plus_flag == false)
		{
			data++;
			plus_flag = true;
		}
		*new_data = *data;
		new_data++;
		data++;
	}
	*new_data = '\0';
	return (new_data - (data_len - 1));
}

char	*append_export_line(char *existing, char *append)
{
	char	*new_data;
	int		new_data_len;

	new_data_len = ft_strlen(existing) + ft_strlen(ft_strchr(append, '=')) - 1;
	new_data = (char *)malloc((new_data_len + 1) * sizeof(char));
	if (!new_data)
		return (NULL);
	while (*existing)
	{
		*new_data = *existing;
		new_data++;
		existing++;
	}
	append = ft_strchr(append, '=') + 1;
	while (*append)
	{
		*new_data = *append;
		new_data++;
		append++;
	}
	*new_data = '\0';
	return (new_data - new_data_len);
}
