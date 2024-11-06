//42 HEADER

#include "minishell.h"

void	add_expansion(char **replace, char *exp, int *copy_index, int *index)
{

	if (exp == NULL)
		return ;
	ft_strlcat(*replace, exp, ft_strlen(*replace) + ft_strlen(exp) + 1);
	*copy_index += ft_strlen(exp);
	if (ft_strncmp(exp, "$", ft_strlen(exp)) == 0 && ft_strlen(exp) != 0)
		(*index)++;
	return ;
}

void	reallocate_replacement(t_shell *shell, char **replace, char *expansion)
{
	char *new_replacement; 

	new_replacement = malloc(sizeof(char) * (ft_strlen(*replace) + ft_strlen(expansion) + 10)); //LEAKS?
	if (!new_replacement)
		error_printer(shell, "", MALLOC_FAIL, true);
	ft_strlcpy(new_replacement, *replace, ft_strlen(*replace) + 1); //LEAKS?
	free(*replace);
	*replace = new_replacement;
}

char	*expand_variable(t_shell *shell, char **replacement, char *pointer)
{
	char	*expansion;
	int		len;

	len = ft_strlen(pointer);
	expansion = malloc(sizeof(char) * (len + 1));
	if (!expansion)
		error_printer(shell, "", MALLOC_FAIL, true);
	ft_strlcpy(expansion, pointer, len + 1);
	reallocate_replacement(shell, replacement, expansion);
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
