/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_other_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:22:00 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/22 12:53:24 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// finds the exit value within shell struct
// in case $? was typed
char	*find_exit_value(t_shell *shell, int *index)
{
	char	*result;

	result = ft_itoa(shell->exit_code);
	(*index)++;
	return (result);
}

// the exceptions where we need to "expand" $ into another $
// basically only if 
int	is_exception(t_shell *shell, t_token *token, int index)
{
	if ((token->line[index + 1] == '\0')
		|| (token->line[index + 1] == '\'' && shell->single_quote == true)
		|| (token->line[index + 1] == '\"' && shell->double_quote == true))
	{
		return (true);
	}
	else if (ft_isalnum(token->line[index + 1]) == false
		&& token->line[index + 1] != '?'
		&& token->line[index + 1] != '\''
		&& token->line[index + 1] != '\"')
	{
		return (true);
	}
	else
		return (false);
}

// flips quote variables within shell struct, whenever encountered
int	handle_quotes(t_shell *shell, t_token *token, int index)
{
	if (token->line[index] == '\'' && shell->double_quote == false)
	{
		shell->single_quote = !shell->single_quote;
		token->quotes = true;
		return (true);
	}
	if (token->line[index] == '\"' && shell->single_quote == false)
	{
		shell->double_quote = !shell->double_quote;
		token->quotes = true;
		return (true);
	}
	return (false);
}

// allocates memory for replacement string, sets it 0, and resets quotes
char	*init_replacement(t_shell *shell, t_token *token)
{
	char	*replacement;
	size_t	len;

	len = ft_strlen(token->line);
	replacement = malloc(sizeof(char) * (len + 1));
	if (!replacement)
		error_printer(shell, "", MALLOC_FAIL, true);
	ft_memset(replacement, 0, len + 1);
	return (replacement);
}

//  basically ft_strdup with malloc check
char	*safe_strdup(t_shell *shell, char *arg)
{
	char	*expansion;

	expansion = ft_strdup(arg);
	if (!expansion)
		error_printer(shell, "", MALLOC_FAIL, true);
	return (expansion);
}
