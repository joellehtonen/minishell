/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:17:13 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/18 14:23:57 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	check_content(t_shell *shell, t_token *token);
char	*create_expansion(t_shell *she, t_token *tok, char **replace, int *i);
void	add_expansion(char **replace, char **exp, int *copy_index, int *index);
void	expand_tilde(t_shell *shell, t_token *temp);

// goes thru each token, looking for quotes, dollar signs or tildes
// if encountered, checks if they ought to be expanded
void	expander(t_shell *shell)
{
	t_token	*temp;
	int		len;

	temp = shell->token_pointer;
	while (temp != NULL)
	{
		len = ft_strlen(temp->line);
		if (ft_strchr(temp->line, '\'') != NULL
			|| ft_strchr(temp->line, '\"') != NULL
			|| ft_strchr(temp->line, '$') != NULL)
		{
			check_content(shell, temp);
		}
		if (ft_strchr(temp->line, '~') != NULL && len == 1)
			expand_tilde(shell, temp);
		temp = temp->next;
	}
}

// flips quote-variables and then expands any $ not within single quotes
// otherwise just copies characters into the replacement of the original token
void	check_content(t_shell *shell, t_token *token)
{	
	int		index;
	int		copy_index;
	char	*replacement;
	char	*expansion;

	index = 0;
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
	free(token->line);
	token->line = replacement;
}

// if found, expands $ to its value. else replaces it with empty space
char	*create_expansion(t_shell *shell, t_token *token, \
	char **replacement, int *index)
{
	char	*key;
	char	*value_pointer;
	char	*expansion;
	int		key_len;

	if (is_exception(token, *index) == true)
		return (ft_strdup("$"));
	if (is_quote(token->line[*index + 1]) == true)
	{
		(*index)++;
		return (NULL);
	}
	key_len = calculate_key_len(token, *index + 1);
	key = ft_substr(token->line, (*index + 1), key_len);
	value_pointer = find_variable(shell, key, key_len);
	free(key);
	if (token->line[*index + 1] == '?')
		value_pointer = find_exit_value(shell, index);
	*index += key_len + 1;
	if (!value_pointer)
		return (ft_strdup(""));
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

// expands ~ char into value of $HOME
void	expand_tilde(t_shell *shell, t_token *temp)
{
	char	*value_pointer;
	char	*replacement;
	char	*expansion;

	replacement = ft_strdup("");
	value_pointer = find_variable(shell, "HOME", 4);
	expansion = expand_variable(shell, &replacement, value_pointer);
	add_expansion(&replacement, &expansion, 0, 0);
	free(value_pointer);
	free(temp->line);
	temp->line = replacement;
}
