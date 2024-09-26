/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklimchu <aklimchu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:23:39 by aklimchu          #+#    #+#             */
/*   Updated: 2024/09/25 12:53:42 by aklimchu         ###   ########.fr       */
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
# include <errno.h> // for errno (error identifiers)
# include <fcntl.h> // for open


# define BUFF_SIZE 1024

typedef struct	s_envp
{
	char			*line;
	struct s_envp	*next;
}				t_envp;

typedef struct	s_token
{
	char			*line;
	float			level;
	int				if_command;
	int				if_argument;
	//int				token_number; do we need the number of tokens? maybe not
	struct s_token	*next;
}				t_token;

typedef struct	s_shell
{
	t_envp	*envp_copy;
	t_envp	*path; //PATH from envp
	char	*uname; //USER from envp
	char	*pwd; //current location
	char	*home; //HOME from envp
	t_token	*token_pointer; //pointer to the head of the linked list that contains the arguments parsed from user input?
	char 	*user_input; //whatever readline reads is saved into this array
	int		exit_code;
}				t_shell;


int copy_envp(t_shell *shell, t_envp **envp_copy, char *envp[]);
int read_input(t_shell *shell);
// list envp functions
void ft_lstadd_back_envp(t_envp **lst, t_envp *new);
t_envp *ft_lstnew_envp(char *content);
t_envp *ft_lstlast_envp(t_envp *lst);
int copy_path(t_envp **path, t_envp *envp_copy);
int copy_uname(char **uname, t_envp *envp_copy);
int copy_home(char **home, t_envp *envp_copy);
int	env_exec(t_envp *envp_copy);
int	unset_exec(t_envp **envp_copy, char *input);
int	exit_exec(t_shell *shell);
int export_exec(t_envp **envp_copy, char *input);
int	update_pwd(t_envp **envp_copy);
// list token functions
void	ft_lstadd_back_token(t_token **lst, t_token *new);
t_token	*ft_lstnew_token(char *content);
t_token	*ft_lstlast_token(t_token *lst);
void	print_node(t_token *lst);
// builtin functions
int	builtins(t_shell *shell);
int	cd_exec(t_shell *shell);
char *get_pwd(char *home);
int	too_many_arg_cd(char *input);
int	only_spaces(char *str);
int	is_directory(char *path);
// miscellaneous
int ft_split_list(t_envp **path, char const *s, char c);
char *ft_strjoin_four(char const *s1, char const *s2, char const *s3, char const *s4);
void	printing(char *cmd, char *dest, char *result, int fd);

#endif /* MINISHELL_H */