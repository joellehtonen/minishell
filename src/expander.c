// 42 HEADER

#include "../inc/minishell.h"

static void	handle_dollar(t_shell *shell, t_token *token, int index)
{
	int	len;
	char *ref;

	len = 0;
	if (token->single_quote != false)
	{
		//look for where the $WORD ends, consider that it can be separated either by space, | or <>. 
		//look for that word in the env, counting len after =
		//malloc for that amount and copy those letters into a new string
		//copy the new string, with $ expanded, into *replacement
		//so "this is my $HOME" becomes "this is my /home/jlehtone"
		//if envp not found, just return
	}

}


static void	handle_quotes(t_shell *shell, t_token *token)
{	
	int	index;
	int	copy_index;
	char *replacement;

	index = 0;
	copy_index = 0;
	replacement = malloc(sizeof(char) * (ft_strlen(token->line) + 1));
	if (!replacement)
		error_printer(shell, MALLOC_FAIL, true);
	while (token->line[index] != '\0')
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
		// if (token->line[index] == '$' && token->line[index + 1] != '\'')
		if (token->line[index] == '$' && ft_isspace)
			handle_dollar(shell, token, index);
		else if (token->line[index] == '$' && token->line[index + 1] == '\'')
			index++;
		else
			replacement[copy_index++] = token->line[index++];
		// if ((double_quote == true || single_quote == false && double_quote == false)
		// 	&& token->line[index] == '$')
	}
	replacement[copy_index] = '\0';
	//free token->line
	//replace with replacement
}


void	expander(t_shell *shell)
{   
	t_token *temp;

	temp = shell->token_pointer;
	while (temp != NULL)
	{
		handle_quotes(shell, temp);
		// if (ft_strchr(temp->line, '$') != NULL)
		// 	replace_dollar(shell, temp);
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