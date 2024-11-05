//42 HEADER

#include "minishell.h"

char	*find_exit_value(t_shell *shell, int *index)
{
	char	*result;

	result = ft_itoa(shell->exit_code);
	(*index)++;
	return (result);
}

int	is_exception(t_token *token, int index)
{
	if (token->line[index + 1] == '\0')
		return (true);
	else if (ft_isalnum(token->line[index + 1]) == false
		&& token->line[index + 1] != '?')
		return (true);
	else
		return (false);
}

int	handle_quotes(t_shell *shell, t_token *token, int index)
{
	if (token->line[index] == '\'' && shell->double_quote == false)
	{
		shell->single_quote = !shell->single_quote;
		return (true);
	}
	if (token->line[index] == '\"' && shell->single_quote == false)
	{
		shell->double_quote = !shell->double_quote;
		return (true);
	}
	return (false);
}

char	*init_replacement(t_shell *shell, t_token *token)
{
	char	*replacement;

	reset_quotes(shell);
	replacement = malloc(sizeof(char) * (ft_strlen(token->line) + 1));
	if (!replacement)
		error_printer(shell, MALLOC_FAIL, true);
	ft_memset(replacement, 0, ft_strlen(token->line) + 1);
	return (replacement);
}
