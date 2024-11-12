/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 08:50:15 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/12 11:29:26 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	fill_values_before_prompt(t_shell **shell)
{
	char	*new_pwd;	
	
	new_pwd = get_pwd((*shell)->home, *shell);
	if (new_pwd)
	{
		free_str(&(*shell)->pwd);
		(*shell)->pwd = new_pwd;
	}
	(*shell)->envp_str = NULL;
	(*shell)->exec = NULL;
	(*shell)->only_one_builtin = 0;
	if (copy_path(&(*shell)->path, (*shell)->envp_copy) == 1)
		return (1);
	if (copy_home(&(*shell)->home, (*shell)->envp_copy) == 1)
		return (1);
	return (copy_uname(&(*shell)->uname, (*shell)->envp_copy));
}