/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:43:33 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/20 14:13:38 by jlehtone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	count_envp(char *envp[]);

int copy_envp(t_envp **envp_copy, char *envp[])
{
	int		i;
	t_envp	*new;

	//(void)envp_copy;
	//(void)envp;
	i = 0;
	envp_copy = (t_envp **)malloc((count_envp(envp) + 1) * sizeof(t_envp));
	while (envp[i])
	{
		new = ft_lstnew_ms(envp[i]);
		//printf("%s\n", new->line);
		if (new == NULL)
		{
			//free_lists(...);
			return(1);
		}
		ft_lstadd_back_ms(envp_copy, new);
		//printf("%s\n", (*envp_copy)->line);
		i++;
	}
	return (0);
}

static int	count_envp(char *envp[])
{
	int		count;
	
	count = 0;
	while (envp[count])
		count++;
	return (count);
}
