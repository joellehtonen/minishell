/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:23:39 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/01 14:21:44 by jlehtone         ###   ########.fr       */
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
# define GETCWD_FAIL "Getting current working directory failed"
# define CHDIR_ERROR "Changing working directory failed"
# define DUP2_ERROR "Failed to duplicate a file descriptor"
# define SIGNAL_ERROR "Failed to set up a signal"
# define SYNTAX_ERROR "Syntax error near unexpected token"
# define NUMERIC_ERROR "Numeric argument required"
# define TOO_MANY_ERROR "Too many arguments"

enum e_success
{
	SUCCESS,
	FAILURE
};

enum e_token_type //do we need all of these? we need to cull what we dont need, sooner or later
{
	COMM = 1,
	ARG,
	REDIR_INPUT,
	REDIR_OUTPUT,
	DELIM, //needed?
	INPUT,
	OUTPUT,
	PIPE, 
	FLAG, //needed?
	SPACES
};

enum e_quotes
{
	S_QUOTE = 1,
	D_QUOTE
};

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
	int				token_number;
	int				combine;
	struct s_token	*next;
}				t_token;

typedef struct	s_exec
{
	int		**pipe;
	int		in;
	int		out;
	int		status;
	int		pipe_num;
	int		pipe_flag;
	int		**here_doc_pipe;
	int		here_doc_num;
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
	t_exec	*exec; // file descriptors for pipes and forks
	char 	*user_input; //whatever readline reads is saved into this array
	t_token	*token_pointer; //pointer to the head of the linked list containing the arguments parsed from user input
	int		single_quote; //whether single quotes are "active"
	int		double_quote; //whether double quotes are "active"
	int		exit_code; 
}				t_shell;

// reading input
int read_input(t_shell *shell);
int input_error_check(t_shell *shell);
// list envp functions
int copy_envp(t_shell *shell, t_envp **envp_copy, char *envp[]);
void ft_lstadd_back_envp(t_envp **lst, t_envp *new);
t_envp *ft_lstnew_envp(char *content);
t_envp	*ft_lstnew_envp_no_strdup(char *content);
t_envp *ft_lstlast_envp(t_envp *lst);
int copy_path(t_envp **path, t_envp *envp_copy);
int copy_uname(char **uname, t_envp *envp_copy);
int copy_home(char **home, t_envp *envp_copy);
int	update_pwd(t_envp **envp_copy, t_shell *shell);
int	update_old_pwd(t_envp **envp_copy, t_shell *shell);
void envp_remove_if_line(t_envp **lst, char *data, int (*cmp)());
void envp_remove_if_export(t_envp **lst, char *data, int (*cmp)());
// list token functions
void	ft_lstadd_back_token(t_token **lst, t_token *new);
t_token	*ft_lstnew_token(char *content);
t_token	*ft_lstlast_token(t_token *lst);
void	delete_one_token(t_token *lst);
void	delete_all_tokens(t_token **lst);
void	print_node(t_token *lst);
// builtin functions
int	exec_builtins(t_shell *shell, int loop_count);
int	cd_exec(t_shell *shell, t_token *cd, int loop_count);
char *get_pwd(char *home, t_shell *shell);
int echo(t_shell *shell, t_token *echo_pointer);
// int echo_exec(t_shell *shell, char *input);
int	env_exec(t_envp *envp_copy);
int	unset_exec(t_envp **envp_copy, t_token *unset, int loop_count);
int	exit_exec(t_shell *shell, t_token *token);
int	export_exec(t_envp **envp_copy, t_token *export, int loop_count);
int	if_builtin(t_shell *shell, int loop_count);
char *get_new_path(t_shell *shell, t_token *arg);
int	is_directory_new(char *path);
// parsing functions
void tokenize_input(t_shell *shell);
int is_IO(char c);
int is_quote(char c);
int is_space(char c);
int is_delim(char c);
int	is_valid_redir(t_shell *shell, int index1, int index2);
void assign_type(t_token **token);
void assign_level(t_token **token, t_exec **exec, t_shell *shell);
void	expander(t_shell *shell);
void reset_quotes(t_shell *shell);
// execute functions
int	execute(t_shell *shell);
void get_input_and_output(t_shell **shell, int loop_count);
char **check_param(t_shell *shell, int loop_count);
char **param_to_arr(t_token *token, int loop_count);
char *check_path(t_envp *paths, char **param, t_exec exec);
void check_command_access(char **param, t_exec exec);
int	is_directory(char *path, t_exec fd, int fd_pipe, char **param);
int is_file(char *path);
int	pipe_and_fork(t_shell *shell, int i);
char **envp_to_arr(t_envp *envp_copy);
void child_process(t_shell **shell, int loop_count);
int	close_free(int pipe_flag, int fd2, int fd3, pid_t **pid);
void close_pipes_child(int loop_count, t_exec **exec);
int	assign_exec_values(t_shell *shell);
int here_doc(t_shell *shell);
t_token	*find_here_doc_token(t_token *token);
int	check_for_input(t_shell *shell, t_token *token, int loop_count, int input_flag);
int	check_for_output(t_shell *shell, t_token *token, int loop_count, int output_flag);
void	allocate_here_doc(t_exec *exec, t_shell *shell);
void check_file_access(t_shell *shell, char	*path, int loop_count);
void	check_all_files(t_token *token, int loop_count);
// miscellaneous
t_token	*find_token(t_token *token, int loop_count, int token_type);
t_token	*find_token_line(t_token *token, int loop_count, int token_type, char *line);
int	count_nodes_type(t_token *start, int token_type, int loop_count);
int ft_split_list(t_envp **path, char const *s, char c);
char *ft_strjoin_four(char const *s1, char const *s2, char const *s3, char const *s4);
void printing(char *cmd, char *dest, char *result, int fd);
void set_up_signals(t_shell *shell);
void clear_input(int signal);
size_t	ft_strchr_fix(const char *s, int c);
// exit
void free_and_exit(t_shell *shell, int error);
void free_double_arr(char **arr);
void error_printer(t_shell *shell, char *message, int exit);
int free_all(char **arr_1, char **arr_2, char *str, pid_t **pid);
int	free_two_str(char *str1, char *str2);
int	free_str(char *str1);
int free_exec(t_exec **exec);

#endif /* MINISHELL_H */