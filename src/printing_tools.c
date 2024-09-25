/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:30:46 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/25 08:50:17 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	printing(char *cmd, char *dest, char *result, int fd)
{
	//ft_putstr_fd("pipex: ", fd);
	ft_putstr_fd(cmd, fd);
	ft_putstr_fd(dest, fd);
	ft_putstr_fd(result, fd);
}