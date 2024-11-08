/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_tools_extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:39:23 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/08 12:41:22 by aklimchu         ###   ########.fr       */
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

void	envp_remove_if_export(t_envp **lst, char *data, int (*cmp)())
{
	t_envp	*temp;

	if (lst == NULL || *lst == NULL)
		return ;
	temp = *lst;
	if (cmp(temp->line, data, ft_strlen(data) \
			- ft_strlen(ft_strchr(data, '=')) + 1) == 0)
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
