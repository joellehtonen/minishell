/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:57:05 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/07 12:58:19 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_valid_redir(t_shell *shell, int index1, int index2)
{
	char	c1;
	char	c2;

	c1 = shell->user_input[index1];
	c2 = shell->user_input[index2];
	if (is_io(c2) == false)
		return (true);
	if ((c1 == '<' && c2 == '<')
		|| (c1 == '<' && c2 == '>')
		|| (c1 == '>' && c2 == '>'))
		return (true);
	else
		return (false);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (true);
	else
		return (false);
}

int	is_io(char c)
{
	if (c == '|')
		return (PIPE);
	if (c == '<')
		return (REDIR_INPUT);
	if (c == '>')
		return (REDIR_OUTPUT);
	else
		return (false);
}

int	is_quote(char c)
{
	if (c == '\'')
		return (S_QUOTE);
	if (c == '\"')
		return (D_QUOTE);
	return (false);
}

void	reset_quotes(t_shell *shell)
{
	shell->single_quote = false;
	shell->double_quote = false;
}
