/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 13:17:13 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/13 10:11:10 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	check_content(t_shell *shell, t_token *token);
char	*create_expansion(t_shell *she, t_token *tok, char **replace, int *i);
void	add_expansion(char **replace, char **exp, int *copy_index, int *index);
void	expand_tilde(t_shell *shell, t_token *temp);

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
			expansion = create_expansion(shell, token, &replacement, &index);
			add_expansion(&replacement, &expansion, &copy_index, &index);
		}
		else
			replacement[copy_index++] = token->line[index++];
	}
	replacement[copy_index] = '\0';
	free(token->line);
	token->line = replacement;
}

char	*create_expansion(t_shell *she, t_token *tok, char **replace, int *i)
{
	char	*key;
	char	*value_pointer;
	char	*expansion;
	int		key_len;

	if (is_exception(tok, *i) == true)
		return (ft_strdup("$"));
	if (is_quote(tok->line[*i + 1]) == true)
	{
		(*i)++;
		return (NULL);
	}
	tok->expanded = true;
	key_len = calculate_key_len(tok, *i + 1);
	key = ft_substr(tok->line, (*i + 1), key_len);
	value_pointer = find_variable(she, key, key_len);
	free(key);
	if (tok->line[*i + 1] == '?')
		value_pointer = find_exit_value(she, i);
	*i += key_len + 1;
	if (!value_pointer)
		return (ft_strdup(""));
	expansion = expand_variable(she, replace, value_pointer, tok);
	free(value_pointer);
	return (expansion);
}

void	add_expansion(char **replace, char **exp, int *copy_index, int *index)
{
	size_t	len;

	if (exp == NULL)
		return ;
	len = ft_strlen(*replace) + ft_strlen(*exp) + 1;
	ft_strlcat(*replace, *exp, len);
	if (copy_index != NULL)
		*copy_index += ft_strlen(*exp);
	if (index != NULL)
	{
		if (ft_strncmp(*exp, "$", ft_strlen(*exp)) == 0 && ft_strlen(*exp) != 0)
			(*index)++;
	}
	free(*exp);
	return ;
}

void	expand_tilde(t_shell *shell, t_token *temp)
{
	char	*value_pointer;
	char	*replacement;
	char	*expansion;

	replacement = ft_strdup("");
	value_pointer = find_variable(shell, "HOME", 4);
	expansion = expand_variable(shell, &replacement, value_pointer, temp);
	add_expansion(&replacement, &expansion, 0, 0);
	free(value_pointer);
	free(temp->line);
	temp->line = replacement;
}
