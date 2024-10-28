// 42 HEADER

#include "../../inc/minishell.h"

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

static char	*handle_dollar(t_shell *shell, t_token *token, int index, int key_len)
{
	char	*key;
	char	*value_pointer;
	char	*expansion;

	key = ft_substr(token->line, index + 1, key_len);
	value_pointer = find_variable(shell, key, key_len);
	free(key);
	if (!value_pointer)
		return (ft_strdup(""));
	expansion = expand_variable(shell, value_pointer);
	free(value_pointer);
	return (expansion);
}

static int expand_dollar(t_shell *shell, t_token *token, char *replacement, int index, int copy_index)
{
	int		key_len;
	char	*expansion;

	if (isquote(token->line[index + 1]) == true 
		|| token->single_quote == true
		|| token->line[index + 1] != '\0')
		return (index);
	key_len = calculate_key_len(token, index + 1);
	expansion = handle_dollar(shell, token, index, key_len);
	ft_strlcat(replacement, expansion, ft_strlen(replacement) + ft_strlen(expansion) + 1);
	copy_index += ft_strlen(expansion);
	index += key_len + 1;
	free(expansion);
	return (index);
}

// static int calc_key_value(t_shell *shell, t_token *token, int index)
// {
// 	int		key_len;

// 	if (isquote(token->line[index + 1]) == true 
// 		|| token->single_quote == true
// 		|| token->line[index + 1] != '\0')
// 		return (index);
// 	key_len = calculate_key_len(token, index + 1);
// }

static int handle_quotes(t_token *token, int index)
{
	if (token->line[index] == '\'' && token->double_quote == false)
	{
		token->single_quote = !token->single_quote;
		index++;
	}
	if (token->line[index] == '\"' && token->single_quote == false)
	{
		token->double_quote = !token->double_quote;
		index++;
	}
	return (index);
}

static char* init_replacement(t_shell *shell, t_token *token)
{
	char *replacement;

	token->double_quote = false;
	token->single_quote = false;
	replacement = malloc(sizeof(char) * (ft_strlen(token->line) + 1));
	if (!replacement)
		error_printer(shell, MALLOC_FAIL, true);
	ft_memset(replacement, 0, ft_strlen(replacement));
	return (replacement);
}

static void	check_content(t_shell *shell, t_token *token)
{	
	int		index;
	int		copy_index;
	char	*replacement;

	index = 0;
	copy_index = 0;
	replacement = init_replacement(shell, token);
	while (token->line[index] != '\0')
	{
		index = handle_quotes(token, index);
		if (token->line[index] == '$')
		{
			copy_index = calc_key_value();
			index = expand_dollar(shell, token, replacement, &index, &copy_index);
		}
		else if (token->line[index] == '$' && isquote(token->line[index + 1]) == true)
			index++;
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
