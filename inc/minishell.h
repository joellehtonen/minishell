/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:23:39 by aklimchu          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/09/20 13:00:57 by aklimchu         ###   ########.fr       */
=======
/*   Updated: 2024/09/20 13:22:37 by jlehtone         ###   ########.fr       */
>>>>>>> remotes/origin/main
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h> //for printf and readline
# include <readline/readline.h> //for readline
# include <readline/history.h> //for readline
# include <stdbool.h> //for true and false
# include <signal.h> //for kill and other signals
# include <stdlib.h> //for exit status and malloc

typedef struct	s_envp
{
	char			*line;
	struct s_envp	*next;
}				t_envp;

typedef struct	s_token
{
	char			*line;
	int				if_command;
	int				if_argument;
	struct s_token	*next;
}				t_token;

typedef struct	s_shell
{
	t_envp	*envp_copy;
<<<<<<< HEAD
	t_envp	*path;
	char	*uname;
}				t_shell;


int copy_envp(t_shell *shell, t_envp **envp_copy, char *envp[]);
=======
	t_token	*token_pointer; //pointer to the head of the linked list that contains the arguments parsed from user input?
	char 	*user_input; //whatever readline reads is saved into this array
	int		exit_code;
}				t_shell;

int read_input(t_shell *shell);
int copy_envp(t_envp **envp_copy, char *envp[]);
>>>>>>> remotes/origin/main
void ft_lstadd_back_ms(t_envp **lst, t_envp *new);
t_envp *ft_lstnew_ms(char *content);
t_envp *ft_lstlast_ms(t_envp *lst);
int ft_split_list(t_envp **path, char const *s, char c);

#endif /* MINISHELL_H */