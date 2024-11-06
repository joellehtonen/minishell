// 42 HEADER

#include "../../inc/minishell.h"

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
	expansion = expand_variable(she, replace, value_pointer);
	free(value_pointer);
	return (expansion);
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
			add_expansion(&replacement, expansion, &copy_index, &index);
			free(expansion);
		}
		else
			replacement[copy_index++] = token->line[index++]; //LEAKS?
	}
	replacement[copy_index] = '\0';
	free(token->line);
	token->line = replacement;
}

void	expander(t_shell *shell)
{
	t_token	*temp;

	temp = shell->token_pointer;
	while (temp != NULL)
	{
		if (ft_strchr(temp->line, '\'') != NULL
			|| ft_strchr(temp->line, '\"') != NULL
			|| ft_strchr(temp->line, '$') != NULL)
		{
			check_content(shell, temp);
		}
		temp = temp->next;
	} 
}
