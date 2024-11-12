/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_tools_extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:39:23 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/12 09:25:27 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	check_len(char *line, char *data);

void	envp_remove_if_line(t_envp **lst, char *data, int (*cmp)())
{
	t_envp	*temp;
	int		len;

	if (lst == NULL || *lst == NULL)
		return ;
	temp = *lst;
	len = check_len(temp->line, data);
	if (cmp(temp->line, data, len) == 0)
	{
		*lst = temp->next;
		free(temp->line);
		free(temp);
		envp_remove_if_line(lst, data, cmp);
	}
	else
	{
		temp = *lst;
		envp_remove_if_line(&temp->next, data, cmp);
	}
}

static int	check_len(char *line, char *data)
{
	int	len;

	if (ft_strlen(line) - ft_strlen(ft_strchr(line, '=')) > \
		ft_strlen(data))
		len = ft_strlen(line) - ft_strlen(ft_strchr(line, '='));
	else
		len = ft_strlen(data);
	return (len);
}

void	envp_remove_if_export(t_envp **lst, char *data, int (*cmp)())
{
	t_envp	*temp;
	char	c;
	int		len;

	if (lst == NULL || *lst == NULL)
		return ;
	temp = *lst;
	c = choose_char(data);
	len = ft_strlen(data) - ft_strlen(ft_strchr(data, c));
	if (c == '=')
		len++;
	if (cmp(temp->line, data, len) == 0)
	{
		*lst = temp->next;
		free(temp->line);
		free(temp);
		envp_remove_if_export(lst, data, cmp);
	}
	else
	{
		temp = *lst;
		envp_remove_if_export(&temp->next, data, cmp);
	}
}

char	choose_char(char *data)
{
	int	i;

	i = 0;
	while (*data)
	{
		if (*data == '=')
			break ;
		data++;
		i++;
	}
	if (i > 0 && *(data - 1) == '+')
		return ('+');
	return ('=');
}

t_envp	*find_envp_line(t_envp *envp, char *line)
{
	t_envp	*temp;
	int		len;
	char	c;

	temp = envp;
	c = choose_char(line);
	len = ft_strlen(line) - ft_strlen(ft_strchr(line, c));
	if (c == '=')
		len++;
	while (temp)
	{
		if (ft_strncmp(temp->line, line, len) == 0 && \
			*(temp->line + len) && *(temp->line + len) == '=')
			return (temp);
		temp = temp->next;
	}
	return ((void *) 0);
}
