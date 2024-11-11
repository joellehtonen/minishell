/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_expansion_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:17:47 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/11 13:05:29 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	realloc_space(t_shell *sh, char **replace, char *exp, t_token *tok)
{
	char	*new_replacement;
	size_t	size_now;
	size_t	new_size;

	size_now = ft_strlen(tok->line) + 1;
	new_size = size_now + ft_strlen(exp) + 1;
	if (size_now >= new_size - 1)
		return ;
	new_replacement = malloc(sizeof(char) * new_size);
	if (!new_replacement)
		error_printer(sh, "", MALLOC_FAIL, true);
	ft_memset(new_replacement, 0, new_size);
	ft_strlcpy(new_replacement, *replace, size_now);
	free(*replace);
	*replace = NULL;
	*replace = new_replacement;
}

char	*expand_variable(t_shell *sh, char **replace, char *ptr, t_token *tok)
{
	char	*expansion;
	int		len;

	len = ft_strlen(ptr);
	expansion = malloc(sizeof(char) * (len + 1));
	if (!expansion)
		error_printer(sh, "", MALLOC_FAIL, true);
	ft_strlcpy(expansion, ptr, len + 1);
	realloc_space(sh, replace, expansion, tok);
	return (expansion);
}

char	*find_variable(t_shell *shell, char *key, int len)
{
	t_envp	*temp;
	char	*value;

	temp = shell->envp_copy;
	value = NULL;
	while (temp)
	{
		if (ft_strncmp(temp->line, key, len) == 0 \
			&& temp->line[len] == '=')
		{
			value = ft_strdup(temp->line + len + 1);
			break ;
		}
		temp = temp->next;
	}
	return (value);
}

int	calculate_key_len(t_token *token, int index)
{
	int	key_len;

	key_len = 0;
	while (token->line[index + key_len] != '\0' \
		&& (ft_isalnum(token->line[index + key_len]) == true \
			|| token->line[index + key_len] == '_'))
	{
		key_len++;
	}
	return (key_len);
}
