/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:01:35 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/19 10:45:33 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	error_check_export(char *line);
static void	export_exec_extra(char *line, t_envp **temp, t_shell *shell);
static void	edit_envp(t_envp *temp, t_token *arg, t_shell *shell);
static int	export_exec_cont(char *line, t_envp **temp, t_shell *s, int alloc);

// The function executes "export" builtin
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
	arg = exp->next;
	edit_envp(temp, arg, shell);
	return (shell->exec->export_exit);
}

// The function checks the input for errors
static int	error_check_export(char *str)
{
	if (ft_strncmp(str, "=\0", 2) == 0)
		return (1);
	if (only_digits_or_empty(str) == 1)
		return (1);
	if (*str > 47 && *str < 58)
		return (1);
	if (*str == '=')
		return (1);
	if (forbidden_symbols(str) == 1)
		return (1);
	return (check_str(str, -1, -1, -1));
}

// The function edits minishell environment variables
static void	edit_envp(t_envp *temp, t_token *arg, t_shell *shell)
{
	while (arg && arg->type == ARG)
	{
		if (error_check_export(arg->line) == 1)
		{
			error_printer(shell, arg->line, NOT_VALID_IDENT, false);
			shell->exec->export_exit = 1;
		}
		else if (ft_strchr(arg->line, '='))
			export_exec_extra(arg->line, &temp, shell);
		arg = arg->next;
	}
}

// The function chooses the scenario of creating a new node
// or expanding existing environmental variable
static void	export_exec_extra(char *line, t_envp **temp, t_shell *shell)
{
	t_envp	*existing_node;
	int		alloc_flag;

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
	export_exec_cont(line, temp, shell, alloc_flag);
}

// The function adds a new environmental variable
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
