/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_expansion_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:17:47 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/22 14:46:47 by aklimchu         ###   ########.fr       */
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
			if (!value)
				error_printer(shell, "", MALLOC_FAIL, true);
			break ;
		}
		temp = temp->next;
	}
	return (value);
}

// copies and returns the expansion of $ variable
char	*expand_variable(t_shell *shell, t_token *token, \
	char **replacement, char *ptr)
{
	char	*expansion;
	int		len;

	len = ft_strlen(ptr);
	expansion = malloc(sizeof(char) * (len + 1));
	if (!expansion)
		error_printer(shell, "", MALLOC_FAIL, true);
	ft_strlcpy(expansion, ptr, len + 1);
	realloc_replacement(shell, token, replacement, expansion);
	return (expansion);
}

// reallocates more memory for the replacement string, 
// if expansions take more space than initially allocated
void	realloc_replacement(t_shell *shell, t_token *token, \
	char **replacement, char *expansion)
{
	char	*new_replacement;
	size_t	size_now;
	size_t	new_size;
	size_t	original_size;

	original_size = ft_strlen(token->line);
	size_now = ft_strlen(*replacement) + 1;
	new_size = size_now + ft_strlen(expansion);
	if (size_now >= new_size)
		return ;
	new_replacement = malloc(sizeof(char) * (new_size + original_size));
	if (!new_replacement)
		error_printer(shell, "", MALLOC_FAIL, true);
	ft_memset(new_replacement, 0, new_size);
	ft_strlcpy(new_replacement, *replacement, size_now);
	free(*replacement);
	*replacement = NULL;
	*replacement = new_replacement;
}

// expands ~ char into value of $HOME
// unless previous token was <<
void	expand_tilde(t_shell *shell, t_token *token, int here_doc)
{
	char	*value_pointer;
	char	*replacement;
	char	*expansion;

	if (here_doc == false)
	{
		replacement = ft_strdup("");
		if (!replacement)
			error_printer(shell, "", MALLOC_FAIL, true);
		value_pointer = find_variable(shell, "HOME", 4);
		expansion = expand_variable(shell, token, &replacement, value_pointer);
		add_expansion(&replacement, &expansion, 0, 0);
		free(value_pointer);
		free(token->line);
		token->line = replacement;
	}
}
