/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 13:11:22 by jlehtone          #+#    #+#             */
/*   Updated: 2024/11/22 13:14:37 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_token	*create_temp_token(t_shell *shell, char *input, int len);
static char		*here_doc_expansion_loop(t_shell *shell, t_token *token, \
	int index);

// expands here_doc input variable to its value if delim is not in quotes
void expand_here_doc(t_shell *shell, char **input)
{
	char	*replacement;
	char	*replacement_with_newline;
	int		index;
	int		len;
	t_token	*temp;
	
	if (ft_strchr(*input, '$') == NULL)
		return ;
	len = ft_strlen(*input);
	temp = create_temp_token(shell, *input, len);
	index = 0;
	reset_quotes(shell);
	replacement = here_doc_expansion_loop(shell, temp, index); // CHECK FOR LEAKS!
	replacement_with_newline = ft_strjoin(replacement, "\n");
	delete_one_token(temp);
	free(replacement);
	free(*input);
	*input = replacement_with_newline;
}

// creates a temporary token so that expander functions will work correctly
static t_token *create_temp_token(t_shell *shell, char *input, int len)
{
	t_token *temp;
	
	temp = ft_lstnew_token(NULL);
	if (temp == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
	temp->line = malloc(sizeof(char) * (len + 1));
	if (temp->line == NULL)
		error_printer(shell, "", MALLOC_FAIL, true);
	ft_strlcpy(temp->line, input, len);
	return (temp);
}

static char	*here_doc_expansion_loop(t_shell *shell, t_token *token, \
	int index)
{
	char	*expansion;
	char	*replacement;
	int		copy_index;

	copy_index = 0;
	replacement = init_replacement(shell, token);
	while (token->line[index] != '\0')
	{
		if (token->line[index] == '$')
		{
			expansion = create_expansion(shell, token, &replacement, &index);
			add_expansion(&replacement, &expansion, &copy_index, &index);
		}
		else
		{
			replacement[copy_index++] = token->line[index++];
			replacement[copy_index] = '\0';
		}
	}
	replacement[copy_index] = '\0';
	return (replacement);
}
