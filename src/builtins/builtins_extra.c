/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_extra.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 10:34:20 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/12 10:16:49 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	env_exec(t_envp *envp, t_shell *shell, t_token *env, int loop)
{
	t_token	*arg;

	arg = find_token(env, loop, ARG);
	if (arg)
	{
		error_printer(shell, arg->line, NO_FILE_DIR_COMM, true);
		return (127);
	}
	while (envp)
	{
		printf("%s\n", envp->line);
		envp = envp->next;
	}
	free_and_exit(shell, 0);
	return (0);
}

int	unset_exec(t_envp **envp_copy, t_token *unset, int loop_count)
{
	t_token	*temp;
	
	loop_count = loop_count; //REMOVE THIS?
	temp = unset->next;
	while (temp && temp->type == ARG)
	{
		envp_remove_if_line(envp_copy, temp->line, ft_strncmp);
		temp = temp->next;
	}
	// arg = find_token(unset, loop_count, ARG);
	// while (arg)
	// {
	// 	envp_remove_if_line(envp_copy, arg->line, ft_strncmp);
	// 	arg = find_token(unset, loop_count, ARG);
	// }
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

char	*append_export_line(char *existing, char *to_append)
{
	char	*new_data;
	int		new_data_len;
	
	new_data_len = ft_strlen(existing) + ft_strlen(ft_strchr(to_append, '=')) - 1;
	new_data = (char *)malloc((new_data_len + 1) * sizeof(char));
	if (!new_data)
		return (NULL);
	while (*existing)
	{
		*new_data = *existing;
		new_data++;
		existing++;
	}
	to_append = ft_strchr(to_append, '=') + 1;
	while (*to_append)
	{
		*new_data = *to_append;
		new_data++;
		to_append++;
	}
	*new_data = '\0';
	return (new_data - new_data_len);
}