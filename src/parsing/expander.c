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

static char	*handle_dollar(t_shell *shell, t_token *token, int index, int len)
{
	char	*key;
	char	*value_pointer;
	char	*expansion;

	key = ft_substr(token->line, index + 1, len);
	value_pointer = find_variable(shell, key, len);
	free(key);
	if (!value_pointer)
		return (ft_strdup(""));
	expansion = expand_variable(shell, value_pointer);
	free(value_pointer);
	return (expansion);
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

// static int handle_quotes(t_token *token, int index)
// {
// 	if (token->line[index] == '\'' && token->double_quote == false)
// 	{
// 		token->single_quote = !token->single_quote;
// 		index++;
// 	}
// 	if (token->line[index] == '\"' && token->single_quote == false)
// 	{
// 		token->double_quote = !token->double_quote;
// 		index++;
// 	}
// 	return (index);
// }

static void	check_content(t_shell *shell, t_token *token)
{	
	int		index;
	int		copy_index;
	int		key_len;
	char	*replacement;
	char	*expansion;

	index = 0;
	copy_index = 0;
	key_len = 0;
	token->double_quote = false; //these 
	token->single_quote = false; //can be memset somewhere else
	replacement = malloc(sizeof(char) * (ft_strlen(token->line) + 1));
	if (!replacement)
		error_printer(shell, MALLOC_FAIL, true);
	replacement[0] = '\0';
	while (token->line[index] != '\0')
	{
		// if (isquote(token->line[index]) == true)
		// {
		// 	index = handle_quotes(token, index);
		// 	printf("index is %d\n", index);
		// }
		if (token->line[index] == '\'' && token->double_quote == false)
		{
			token->single_quote = !token->single_quote;
			index++;
		}
		else if (token->line[index] == '\"' && token->single_quote == false)
		{
			token->double_quote = !token->double_quote;
			index++;
		}
		else if (token->line[index] == '$' \
			&& isquote(token->line[index + 1]) == false \
			&& token->line[index + 1] != '\0'
			&& token->single_quote == false)
		{
			key_len = calculate_key_len(token, index + 1);
			expansion = handle_dollar(shell, token, index, key_len);
			ft_strlcat(replacement, expansion, ft_strlen(replacement) + ft_strlen(expansion) + 1);
			index += key_len + 1;
			copy_index += ft_strlen(expansion);
			free(expansion);
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
	t_token *temp;

	temp = shell->token_pointer;
	while (temp != NULL)
	{
		check_content(shell, temp);
		temp = temp->next;
	}
}
