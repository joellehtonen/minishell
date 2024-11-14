/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools_extra_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:14:22 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/14 11:13:04 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// The function changes the SHLVL value in minishell environment
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

// The function modifies the string containing SHLVL value
char	*append_shlvl_line(char *line, int add, t_shell *shell)
{
	char	*new_data;
	int		new_num;
	char	*new_num_str;

	new_num = ft_atoi(ft_strrchr(line, '=') + 1) + add;
	if (new_num <= 0)
		new_num = 1;
	new_num_str = ft_itoa(new_num);
	new_data = ft_strjoin("SHLVL=", new_num_str);
	if (!new_data)
		error_printer(shell, "", MALLOC_FAIL, true);
	return (new_data);
}
