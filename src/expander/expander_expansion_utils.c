//42 HEADER

#include "minishell.h"

void add_expansion(char **replace, char *exp, int *copy_index, int *index)
{
	if (exp == NULL)
		return ;
	ft_strlcat(*replace, exp, ft_strlen(*replace) + ft_strlen(exp) + 1);
	*copy_index += ft_strlen(exp);
	if (ft_strncmp(exp, "$", ft_strlen(exp)) == 0)
		(*index)++;
	return ;
}

char *expand_variable(t_shell *shell, char *pointer)
{
	char	*expansion;
	int		len;

	len = ft_strlen(pointer);
	expansion = malloc(sizeof(char) * (len + 1));
	if (!expansion)
		error_printer(shell, MALLOC_FAIL, true);
	ft_strlcpy(expansion, pointer, len + 1);
	return (expansion);
}

char *find_variable(t_shell *shell, char *key, int len)
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
			&&  (ft_isalnum(token->line[index + key_len]) == true \
			|| token->line[index + key_len] == '_'))
	{
		key_len++;
	}
	return (key_len);
}

char	*create_expansion(t_shell *shell, t_token *token, int *index)
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
	*index += key_len;
	if (!value_pointer)
		return (ft_strdup(""));
	expansion = expand_variable(shell, value_pointer);
	free(value_pointer);
	return (expansion);
}