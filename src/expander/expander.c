// 42 HEADER

#include "../../inc/minishell.h"

// if node is empty after expanding, delete it
static void	clean_empty_node(t_shell *shell, t_token **temp, t_token **prev)
{
	t_token *next;

	next = (*temp)->next;
	if (ft_strlen((*temp)->line) == 0)
	{
		delete_one_token(*temp);
		if ((*prev) == NULL)
			shell->token_pointer = next;
		else
			(*prev)->next = next;
	}
	else
	{
		*prev = *temp;
		*temp = next;
	}
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
			expansion = create_expansion(shell, token, &index);
			add_expansion(&replacement, expansion, &copy_index, &index);
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
	t_token *prev_node;

	temp = shell->token_pointer;
	prev_node = NULL;
	while (temp != NULL)
	{
		if (ft_strchr(temp->line, '\'') != NULL
			|| ft_strchr(temp->line, '\"') != NULL
			|| ft_strchr(temp->line, '$') != NULL)
		{
			check_content(shell, temp);
			clean_empty_node(shell, &temp, &prev_node);
		}
		if (temp != NULL)
		{
			prev_node = temp;
			temp = temp->next;
		}
	}
}
