/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools_extra_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:14:22 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/13 15:15:35 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	change_shlvl(t_envp **envp, int add, t_shell *shell)
{
	t_envp	*temp;
	t_envp	*existing_node;
	char	*line;
	t_envp	*new;

	if (!(*envp))
		return (0);
	temp = *envp;
	existing_node = find_envp_line_simple(temp, "SHLVL=");
	if (!existing_node)
		return (0);
	line = append_shlvl_line(existing_node->line, add, shell);
	envp_remove_if_export(&temp, "SHLVL=", ft_strncmp);
	new = ft_lstnew_envp(line);
	free_str(&line);
	if (new == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
	ft_lstadd_back_envp(&temp, new);
	return (0);
}

char	*append_shlvl_line(char *line, int add, t_shell *shell)
{
	char	*new_data;
	int		old_num;
	char	*new_num;

	old_num = ft_atoi(ft_strrchr(line, '=') + 1); // overflow? extra symbols?
	new_num = ft_itoa(old_num + add);
	new_data = ft_strjoin("SHLVL=", new_num);
	if (!new_data)
		error_printer(shell, "", MALLOC_FAIL, true);
	return (new_data);
}
