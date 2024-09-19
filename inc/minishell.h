/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:23:39 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/19 15:19:06 by aklimchu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>

typedef struct	s_envp
{
	char			*line;
	struct s_envp	*next;
}				t_envp;

typedef struct	s_shell
{
	t_envp	*envp_copy;
}				t_shell;


int copy_envp(t_envp **envp_copy, char *envp[]);
void ft_lstadd_back_ms(t_envp **lst, t_envp *new);
t_envp *ft_lstnew_ms(char *content);
t_envp	*ft_lstlast_ms(t_envp *lst);

#endif /* MINISHELL_H */