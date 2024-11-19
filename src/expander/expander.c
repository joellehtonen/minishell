/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:17:13 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/19 13:04:16 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	check_content(t_shell *shell, t_token *token);
char	*expansion_loop(t_shell *shell, t_token *token, int index);
char	*create_expansion(t_shell *shell, t_token *token, \
	char **replacement, int *index);
void	add_expansion(char **replacement, char **expansion, \
	int *copy_index, int *index);

// goes thru each token, looking for quotes, dollar signs or tildes
// if encountered, checks if they should to be expanded
// not expanding if right after <<
void	expander(t_shell *shell)
{
	t_token	*temp;
	int		len;
	int		here_doc;

	here_doc = false;
	temp = shell->token_pointer;
	while (temp != NULL)
	{
		if ((ft_strchr(temp->line, '\'') != NULL
				|| ft_strchr(temp->line, '\"') != NULL
				|| ft_strchr(temp->line, '$') != NULL)
			&& here_doc == false)
		{
			check_content(shell, temp);
		}
		len = ft_strlen(temp->line);
		if (ft_strchr(temp->line, '~') != NULL && len == 1
			&& here_doc == false)
			expand_tilde(shell, temp);
		if (ft_strncmp(temp->line, "<<\0", 3) == 0)
			here_doc = true;
		else
			here_doc = false;
		temp = temp->next;
	}
}

void	check_content(t_shell *shell, t_token *token)
{	
	char	*replacement;
	int		index;

	index = 0;
	reset_quotes(shell);
	replacement = expansion_loop(shell, token, index);
	free(token->line);
	token->line = replacement;
}

// flips quote-variables and then expands any $ not within single quotes
// otherwise just copies characters into the replacement of the original token
char	*expansion_loop(t_shell *shell, t_token *token, int index)
{
	char	*expansion;
	char	*replacement;
	int		copy_index;

	copy_index = 0;
	replacement = init_replacement(shell, token);
	while (token->line[index] != '\0')
	{
		if (handle_quotes(shell, token, index) == true)
			index++;
		else if (token->line[index] == '$' && shell->single_quote == false)
		{
			token->expanded = true;
			expansion = create_expansion(shell, token, &replacement, &index);
			add_expansion(&replacement, &expansion, &copy_index, &index);
		}
		else
		{
			replacement[copy_index++] = token->line[index++];
			replacement[copy_index] = '\0';
		}
	}
	replacement[copy_index] = '\0';
	return (replacement);
}

// if found, expands $ to its value. else replaces it with empty space
char	*create_expansion(t_shell *shell, t_token *token, \
	char **replacement, int *index)
{
	char	*key;
	char	*value_pointer;
	char	*expansion;
	int		key_len;

	if (is_exception(shell, token, *index) == true)
		return (safe_strdup(shell, "$"));
	key_len = calculate_key_len(token, *index + 1);
	key = ft_substr(token->line, (*index + 1), key_len);
	value_pointer = find_variable(shell, key, key_len);
	free(key);
	if (token->line[*index + 1] == '?')
		value_pointer = find_exit_value(shell, index);
	*index += key_len + 1;
	if (!value_pointer)
		return (safe_strdup(shell, ""));
	expansion = expand_variable(shell, replacement, value_pointer);
	free(value_pointer);
	return (expansion);
}

// adds the expanded part to the end of the current replacement, 
// updates indexes accordingly
void	add_expansion(char **replacement, char **expansion, \
	int *copy_index, int *index)
{
	size_t	len;

	if (expansion == NULL)
		return ;
	len = ft_strlen(*replacement) + ft_strlen(*expansion) + 1;
	ft_strlcat(*replacement, *expansion, len);
	if (copy_index != NULL)
		*copy_index += ft_strlen(*expansion);
	if (index != NULL)
	{
		if (ft_strncmp(*expansion, "$", ft_strlen(*expansion)) == 0
			&& ft_strlen(*expansion) != 0)
			(*index)++;
	}
	free(*expansion);
	return ;
}
