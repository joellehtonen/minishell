/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 12:02:54 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/27 14:39:59 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*delete_quotes(char *input);

static int	calculate_quotes(char *input);

static void	echo_print(t_envp *envp_copy, char *input);

int echo_exec(t_envp *envp_copy, char *input)
{
	int		nl_flag;
	char	**words;
	int		i;

	input = input + 5; // white spaces handled?
	nl_flag = 0;
	if (ft_strncmp(input, "-n", 2) == 0 && *(input + 2) == ' ')
	{
		nl_flag = 1;
		input = input + 3;
	}
	input = delete_quotes(input);
	if (input == NULL)
	{
		printf("\n");
		return (0);
	}
	if (input == NULL)
	{
		perror("malloc error");
		//free_and_exit();
	}
	words = ft_split(input, ' ');
	if (words == NULL)
	{
		perror("malloc error");
		//free_and_exit();
	}
	i = 0;
	while (words[i])
	{
		if (i != 0)
			printf(" ");
		echo_print(envp_copy, words[i++]);
	}
	if (nl_flag == 0)
		printf("%c", '\n');
	//"echo moi $USERNAME" - add this ability when tokens / parsing is ready
	return (0);
}

static char	*delete_quotes(char *input)
{
	char	*new_input;
	int		new_len;
	int		flag_single;
	
	flag_single = 0;
	new_len = calculate_quotes(input);
	if (new_len == 0)
		return (NULL);
	new_input = (char *)malloc(new_len + 1);
	if (new_input == NULL)
	{
		perror("malloc error");
		//free_and_exit();
	}
	while (*input)
	{
		if (*input == '\'' && *(input + 1) == '$' && flag_single == 0)
		{
			flag_single = 1;
			*new_input = *input;
			new_input++;
		}
		else if (*input == '\'' && *(input + 1) != '$' && flag_single == 1)
			flag_single = 0;
		else if (*input == '"' || *input == '\'')
			;
		else
		{
			*new_input = *input;
			new_input++;
		}
		input++;
	}
	*new_input = '\0';
	return(new_input - new_len);
}

static int	calculate_quotes(char *input)
{
	int		quotes;
	int		flag_single;
	int		len;
	
	len = ft_strlen(input);
	quotes = 0;
	flag_single = 0;
	while (*input)
	{
		if (*input == '\'' && *(input + 1) != '$' && flag_single == 0)
			quotes++;
		if (*input == '\'' && *(input + 1) == '$' && flag_single == 1)
			quotes++;
		if (*input == '\'' && *(input + 1) == '$' && flag_single == 0)
			flag_single = 1;
		if (*input == '\'' && *(input + 1) != '$' && flag_single == 1)
		{
			quotes++;
			flag_single = 0;
		}
		if (*input == '"')
			quotes++;
		input++;
	}
	return(len - quotes);
}

static void	echo_print(t_envp *envp_copy, char *input)
{
	int		flag;

	flag = 0;	
	if (*input == '\'')
	{
		flag = 1;
		input++;
	}
	if (*input == '$' && ft_strlen(input) == 1)
		printf("$");
	else if (*input == '$' && flag == 0)
	{
		while (envp_copy)
		{
			if (ft_strncmp(envp_copy->line, input + 1, \
				ft_strlen(input + 1)) == 0)
				printf("%s", envp_copy->line + ft_strlen(input));
			envp_copy = envp_copy->next;
		}
	}
	else
		printf("%s", input);
}
