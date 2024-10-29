// 42 HEADER

#include "../../inc/minishell.h"

static void add_expansion(char *replacement, char *expansion, int *copy_index, int *index)
{
	if (expansion == NULL)
		return ;
	ft_strlcat(replacement, expansion, ft_strlen(replacement) + ft_strlen(expansion) + 1);
	*copy_index += ft_strlen(expansion);
	if (ft_strncmp(expansion, "$", ft_strlen(expansion)) == 0)
		(*index)++;
	return ;
}

static char *expand_variable(t_shell *shell, char *pointer)
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

static char *find_variable(t_shell *shell, char *key, int len)
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

static int	calculate_key_len(t_token *token, int index)
{
	int	key_len;

	key_len = 0;
	while (token->line + index + key_len \
			&& (ft_isalnum(token->line[index + key_len]) == true \
			|| token->line[index + key_len] == '_'))
	{
		key_len++;
	}
	return (key_len);
}

static char	*create_expansion(t_shell *shell, t_token *token, int *index)
{
	char	*key;
	char	*value_pointer;
	char	*expansion;
	int		key_len;

	if (shell->single_quote == true || token->line[*index + 1] == '\0')
		return (ft_strdup("$"));
	if (isquote(token->line[*index + 1]) == true)
	{
		(*index)++;
		return (NULL);
	}
	key_len = calculate_key_len(token, *index + 1);
	key = ft_substr(token->line, (*index + 1), key_len);
	value_pointer = find_variable(shell, key, key_len);
	free(key);
	if (!value_pointer)
		return (ft_strdup(""));
	expansion = expand_variable(shell, value_pointer);
	free(value_pointer);
	*index += key_len + 1;
	return (expansion);
}

static int handle_quotes(t_shell *shell, t_token *token, int index)
{
	if (token->line[index] == '\'' && shell->double_quote == false)
	{
		shell->single_quote = !shell->single_quote;
		index++;
	}
	if (token->line[index] == '\"' && shell->single_quote == false)
	{
		shell->double_quote = !shell->double_quote;
		index++;
	}
	return (index);
}

static char* init_replacement(t_shell *shell, t_token *token)
{
	char *replacement;

	reset_quotes(shell);
	replacement = malloc(sizeof(char) * (ft_strlen(token->line) + 1));
	if (!replacement)
		error_printer(shell, MALLOC_FAIL, true);
	ft_memset(replacement, 0, ft_strlen(token->line) + 1);
	return (replacement);
}

static void	check_content(t_shell *shell, t_token *token)
{	
	int		index;
	int		copy_index;
	char	*replacement;
	char 	*expansion;

	index = 0;
	copy_index = 0;
	replacement = init_replacement(shell, token);
	while (token->line[index] != '\0')
	{
		index = handle_quotes(shell, token, index);
		if (token->line[index] == '$')
		{
			expansion = create_expansion(shell, token, &index);
			add_expansion(replacement, expansion, &copy_index, &index);
			if (expansion)
				free(expansion);
		}
		else
			replacement[copy_index++] = token->line[index++];
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
