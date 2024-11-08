/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:01:35 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/08 10:33:31 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	error_check_export(char *line);

static int	export_exec_extra(char *line, t_envp **temp, t_shell *shell);

static int	only_digits_or_empty(char *str);

int	export_exec(t_envp **envp, t_token *exp, int loop, t_shell *shell)
{
	t_envp	*temp;
	t_token	*arg;

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
	t_envp	*new;

	if (ft_strchr(line, '=') == NULL)
		return (0);
	envp_remove_if_export(temp, line, ft_strncmp);
	new = ft_lstnew_envp(line);
	if (new == NULL)
	{
		error_printer(shell, "", MALLOC_FAIL, true);
		return (1);
	}
	ft_lstadd_back_envp(temp, new);
	return (0);
}

static int	error_check_export(char *str)
{
	int	position_minus;
	int	position_equal;
	int	i;

	if (ft_strncmp(str, "=\0", 2) == 0)
		return (1);
	if (only_digits_or_empty(str) == 1)
		return (1);
	position_minus = -1;
	position_equal = -1;
	i = 0;
	while (str[i])
	{
		if (str[i] == '-')
			position_minus = i;
		if (str[i] == '=')
			position_equal = i;
		if (position_minus > -1 && (position_equal == -1 || \
			position_minus < position_equal))
			return (1);
		i++;
	}
	return (0);
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
