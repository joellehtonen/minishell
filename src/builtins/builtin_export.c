/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:01:35 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/12 09:56:45 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	error_check_export(char *line);
static int	export_exec_extra(char *line, t_envp **temp, t_shell *shell);
static int	export_exec_cont(char *line, t_envp **temp, t_shell *s, int alloc);
static int	only_digits_or_empty(char *str);

int	export_exec(t_envp **envp, t_token *exp, int loop, t_shell *shell)
{
	t_envp	*temp;
	t_token	*arg;

	if (!(*envp))
		return (1);
	temp = *envp;
	arg = find_token(exp, loop, ARG);
	if (!arg)
	{
		while (temp)
		{
			printf("declare -x %s\n", temp->line);
			temp = temp->next;
		}
		return (0);
	}
	if (error_check_export(arg->line) == 1)
	{
		error_printer(shell, arg->line, NOT_VALID_IDENT, true);
		return (1);
	}
	return (export_exec_extra(arg->line, &temp, shell));
}

static int	export_exec_extra(char *line, t_envp **temp, t_shell *shell)
{
	t_envp	*existing_node;
	int		alloc_flag;

	if (ft_strchr(line, '=') == NULL)
		return (0);
	alloc_flag = false;
	existing_node = NULL;
	if (choose_char(line) == '+')
		existing_node = find_envp_line(*temp, line);
	if (choose_char(line) == '+' && existing_node)
	{
		line = append_export_line(existing_node->line, line);
		alloc_flag = true;
	}
	else if (choose_char(line) == '+')
	{
		line = remove_plus(line);
		alloc_flag = true;
	}
	return (export_exec_cont(line, temp, shell, alloc_flag));
}

static int	export_exec_cont(char *line, t_envp **temp, t_shell *s, int alloc)
{
	t_envp	*new;

	if (!line)
	{
		error_printer(s, "", MALLOC_FAIL, true);
		return (1);
	}
	envp_remove_if_export(temp, line, ft_strncmp);
	new = ft_lstnew_envp(line);
	if (alloc == true)
		free_str(&line);
	if (new == NULL)
	{
		error_printer(s, "", MALLOC_FAIL, true);
		return (1);
	}
	ft_lstadd_back_envp(temp, new);
	return (0);
}

static int	error_check_export(char *str)
{
	if (ft_strncmp(str, "=\0", 2) == 0)
		return (1);
	if (only_digits_or_empty(str) == 1)
		return (1);
	return (check_str(str, -1, -1, -1));
}

static int	only_digits_or_empty(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] >= 48 && str[i] <= 57)
			count++;
		i++;
	}
	if (i == count)
		return (1);
	return (0);
}
