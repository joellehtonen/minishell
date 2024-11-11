/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 08:50:15 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/11 09:12:40 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	fill_values_before_prompt(t_shell **shell)
{
	(*shell)->pwd = get_pwd((*shell)->home, *shell);
	(*shell)->envp_str = NULL;
	(*shell)->exec = NULL;
	(*shell)->only_one_builtin = 0;
	if (copy_path(&(*shell)->path, (*shell)->envp_copy) == 1)
		return (1);
	if (copy_home(&(*shell)->home, (*shell)->envp_copy) == 1)
		return (1);
	return (copy_uname(&(*shell)->uname, (*shell)->envp_copy));
}