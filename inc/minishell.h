/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:23:39 by aklimchu          #+#    #+#             */
/*   Updated: 2024/10/02 14:20:17 by jlehtone         ###   ########.fr       */
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
# include <sys/wait.h> // for waitpid

# define BUFF_SIZE 1024

// error messages
# define EMPTY_INPUT "Empty input"
# define PIPE_ERROR "Pipe cannot be first or last, or follow another pipe"
# define REDIR_ERROR "Redir can't come last, or before a pipe or another redir"
# define QUOTE_ERROR "Odd number of quotes (only even amount accepted)"
# define MALLOC_FAIL "Allocating memory failed"
# define SIGNAL_ERROR "Failed to set up a signal"
# define SYNTAX_ERROR "Syntax error near unexpected token"

enum e_success
{
	SUCCESS,
	FAILURE
};

enum e_token_type
{
	COMM = 1,
	ARG,
	REDIR,
	DELIM,
	INPUT,
	OUTPUT,
	PIPE, 
	FLAG, 
	SPACES
};

// enum e_quotes
// {
// 	SINGLE_QUOTE = 1,
// 	DOUBLE_QUOTE
// };

typedef struct	s_envp
{
	char			*line;
	struct s_envp	*next;
}				t_envp;

typedef struct	s_token
{
	char			*line;
	int				level;
	int				type;
	int				token_number; //for error checking at least
	struct s_token	*next;
}				t_token;

typedef struct	s_exec
{
	int		pipe[2];
	int		in;
	int		out;
	int		status;
	int		pipe_num;
	pid_t	*null;
	pid_t	*pid;
}				t_exec;


typedef struct	s_shell
{
	t_envp	*envp_copy;
	char	**envp_str; // envp in the form of double array
	t_envp	*path; //PATH from envp
	char	*uname; //USER from envp
	char	*pwd; //current location
	char	*home; //HOME from envp
	t_token	*token_pointer; //pointer to the head of the linked list that contains the arguments parsed from user input?
	char 	*user_input; //whatever readline reads is saved into this array
	t_exec	*exec; // file descriptors for pipes and forks
	int		exit_code; 
}				t_shell;

// reading input
int read_input(t_shell *shell);
int input_error_check(t_shell *shell);
// list envp functions
int copy_envp(t_shell *shell, t_envp **envp_copy, char *envp[]);
void ft_lstadd_back_envp(t_envp **lst, t_envp *new);
t_envp *ft_lstnew_envp(char *content);
t_envp *ft_lstlast_envp(t_envp *lst);
int copy_path(t_envp **path, t_envp *envp_copy);
int copy_uname(char **uname, t_envp *envp_copy);
int copy_home(char **home, t_envp *envp_copy);
int	update_pwd(t_envp **envp_copy);
// list token functions
void	ft_lstadd_back_token(t_token **lst, t_token *new);
t_token	*ft_lstnew_token(char *content);
t_token	*ft_lstlast_token(t_token *lst);
void	delete_one_token(t_token *lst);
void	delete_all_tokens(t_token **lst);
void	print_node(t_token *lst);
// builtin functions
int	builtins(t_shell *shell);
int	cd_exec(t_shell *shell);
char *get_pwd(char *home);
int	too_many_arg_cd(char *input);
int	only_spaces(char *str);
int	is_directory_new(char *path);
int echo_exec(t_shell *shell, char *input);
int	env_exec(t_envp *envp_copy);
int	unset_exec(t_envp **envp_copy, char *input);
int	exit_exec(t_shell *shell);
int export_exec(t_envp **envp_copy, char *input);
// parsing functions
void tokenize_input(t_shell *shell);
int isIO(t_shell *shell, int index);
int isquote(t_shell *shell, int index);
int isspaces(t_shell *shell, int index);
void assign_type(t_token **token);
void assign_level(t_token **token, t_exec **exec);
// execute functions
int	execute(t_shell *shell);
char **check_param(t_shell *shell, int loop_count);
char **param_to_arr(t_token *token, int loop_count);
char *check_path(t_envp *paths, char **param, t_exec exec);
void check_command_access(char **param, t_exec exec);
int	is_directory(char *path, t_exec fd, int fd_pipe, char **param);
int	pipe_and_fork(t_shell *shell, int i);
char **envp_to_arr(t_envp *envp_copy);
void child_process(t_shell **shell, int loop_count, int flag_pipe);
int free_exec(t_exec **exec);
int	close_free(int fd1, int fd2, int fd3, pid_t **pid);
// miscellaneous
int ft_split_list(t_envp **path, char const *s, char c);
char *ft_strjoin_four(char const *s1, char const *s2, char const *s3, char const *s4);
void printing(char *cmd, char *dest, char *result, int fd);
void	set_up_signals(t_shell *shell);
void	clear_input(int signal);
// exit
void free_and_exit(t_shell *shell, int error);
void free_double_arr(char **arr);
void	error_printer(t_shell *shell, char *message, int exit);

#endif /* MINISHELL_H */