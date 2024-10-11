// 42 HEADER

#include "../inc/minishell.h"

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

static char	*handle_dollar(t_shell *shell, t_token *token, int index)
{
	int		len;
	char	*key;
	char	*value_pointer;
	char	*expansion;

	len = 0;
	while (token->line + index + len \
		&& (ft_isalnum(token->line[index + len]) == true \
		|| token->line[index + len] == '_'))
	{
		len++;
	}
	key = ft_substr(token->line, index + 1, len);
	value_pointer = find_variable(shell, key, len);
	free(key);
	if (!value_pointer)
		return (ft_strdup(""));
	expansion = expand_variable(shell, value_pointer);
	free(value_pointer);
	return (expansion);
}

static int handle_quotes(t_shell *shell, t_token *token, int index)
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

static void	check_content(t_shell *shell, t_token *token)
{	
	int		index;
	int		copy_index;
	char	*replacement;
	char	*expansion;

	index = 0;
	copy_index = 0;
	replacement = malloc(sizeof(char) * (ft_strlen(token->line) + 1));
	if (!replacement)
		error_printer(shell, MALLOC_FAIL, true);
	while (token->line[index] != '\0')
	{
		if (isquote(token->line[index] == true))
			index = handle_quote(shell, token, index);
		if (token->line[index] == '$' \
			&& isquote(token->line[index + 1]) == false \
			&& token->line[index + 1] != '\0' \
			&& token->single_quote == false)
		{
			expansion = handle_dollar(shell, token, index);
			ft_strlcat(replacement, expansion, ft_strlen(replacement) + ft_strlen(expansion) + 1);
			index += ft_strlen(expansion);
			free(expansion);
		}
		else if (token->line[index] == '$' && isquote(token->line[index + 1]) == true)
			index++;
		else
			replacement[copy_index++] = token->line[index++];
	}
	replacement[copy_index] = '\0';
	free (token->line);
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

	// int	index;
	// int	copy_index;
	// char *words_to_copy;
	// int	single_quote;
	// int	double_quote;

	// single_quote = false;
	// double_quote = false;
	// index = 0;
	// copy_index = 0;
	// words_to_copy = ft_strdup("");
	// if (!words_to_copy)
	// 	error_printer(shell, MALLOC_FAIL, true);
	// while (content[index] != '\0')
	// {
	// 	if (content[index] == '\'' && double_quote == false)
	// 	{
	// 		single_quote = !single_quote;
	// 		index++;
	// 	}
	// 	if (content[index] == '\"' && single_quote == false)
	// 	{
	// 		double_quote = !double_quote;
	// 		index++;
	// 	}
	// 	if (single_quote == true)
	// 	{
	// 		words_to_copy[copy_index++] = content[index];
	// 	}
	// 	else if (double_quote == true & content[index] == '$')
	// 	{
	// 		while (shell->envp_copy)
	// 		{
	// 			//if (ft_strncmp(shell->envp_copy->line)
	// 		}
	// 		words_to_copy[copy_index++] = content[index];
	// 	}
	// 	else
	// 		words_to_copy[copy_index++] = content[index];
	// 	index++;
	// }
	// words_to_copy[copy_index] = '\0';
	// ft_strlcat(combination, words_to_copy, ft_strlen(words_to_copy));
	// free(words_to_copy);
	// return (combination);