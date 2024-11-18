/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_expansion_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:17:47 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/18 14:05:54 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// calculates length of the key so that strdup and strncmp can later function
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

// finds the correct variable from envp based on its key
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

// copies and returns the expansion of $ variable
char	*expand_variable(t_shell *shell, char **replacement, char *ptr)
{
	char	*expansion;
	int		len;

	len = ft_strlen(ptr);
	expansion = malloc(sizeof(char) * (len + 1));
	if (!expansion)
		error_printer(shell, "", MALLOC_FAIL, true);
	ft_strlcpy(expansion, ptr, len + 1);
	realloc_replacement(shell, replacement, expansion);
	return (expansion);
}

// reallocates more memory for the replacement string, 
// if expansions take more space than initially allocated
void	realloc_replacement(t_shell *shell, char **replacement, \
	char *expansion)
{
	char	*new_replacement;
	size_t	size_now;
	size_t	new_size;

	size_now = ft_strlen(*replacement) + 1;
	new_size = size_now + ft_strlen(expansion) + 1;
	if (size_now >= new_size - 1)
		return ;
	new_replacement = malloc(sizeof(char) * new_size);
	if (!new_replacement)
		error_printer(shell, "", MALLOC_FAIL, true);
	ft_memset(new_replacement, 0, new_size);
	ft_strlcpy(new_replacement, *replacement, size_now);
	free(*replacement);
	*replacement = NULL;
	*replacement = new_replacement;
}
