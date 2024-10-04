// 42 HEADER

#include "../inc/minishell.h"

void	replace_dollar(t_shell *shell, t_token *token)
{	
	int	index;
	int	copy_index;
	char *replacement;
	int	single_quote;
	int	double_quote;

	index = 0;
	copy_index = 0;
	single_quote = false;
	double_quote = false;
	replacement = malloc(sizeof(char) * (ft_strlen(token->line) + 1));
	if (!replacement)
		error_printer(shell, MALLOC_FAIL, true);
	while (token->line[index] != '\0')
	{
		if (token->line[index] == '\'' && double_quote == false)
		{
			single_quote = !single_quote;
			index++;
		}
		if (token->line[index] == '\"' && single_quote == false)
		{
			double_quote = !double_quote;
			index++;
		}
		if ((double_quote == true || single_quote == false && double_quote == false)
			&& token->line[index] == '$')
		{
			while (shell->envp_copy)
			{
				shell->envp_copy = shell->envp_copy->next;
			}
		}
		if (single_quote == true)
			replacement[copy_index++] = token->line[index];

		index++;
	}
	replacement[copy_index] = '\0';
}

void	find_dollar(t_shell *shell)
{   
	t_token *temp;

	temp = shell->token_pointer;
	while (temp != NULL)
	{
		if (ft_strchr(temp->line, '$') != NULL)
			replace_dollar(shell, temp);
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