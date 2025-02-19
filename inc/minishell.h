/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlehtone <jlehtone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 13:23:39 by aklimchu          #+#    #+#             */
/*   Updated: 2024/11/22 14:20:12 by jlehtone         ###   ########.fr       */
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
# include <termios.h> //for disabling keypresses in signals

# define BUFF_SIZE 1024

// ERROR MESSAGES
# define EMPTY_INPUT "Empty input\n"
# define QUOTE_ERROR "Odd number of quotes (only even amount accepted)\n"
# define MALLOC_FAIL "Allocating memory failed\n"
# define SIGNAL_ERROR "Failed to set up a signal\n"
# define SYNTAX_ERROR "Syntax error near unexpected token\n"
# define NUMERIC_ERROR "Numeric argument required\n"
# define NOT_VALID_IDENT ": not a valid identifier\n"
# define TOO_MANY_ARGS "Too many arguments\n"
# define CMD_NOT_FOUND ": command not found\n"
# define NO_FILE_DIR ": No such file or directory\n"
# define NO_FILE_DIR_COMM ": No such file or directory \n"
# define IS_DIR ": Is a directory\n"
# define IS_DIR_COMM ": Is a directory \n"
# define NOT_DIR " Not a directory\n"
# define PERM_DENIED ": Permission denied\n"
# define PERM_DENIED_COMM ": Permission denied \n"
# define GETCWD_FAIL "Error retrieving current directory\n"
# define CHDIR_ERROR "Changing working directory failed\n"
# define DUP2_ERROR "Failed to duplicate a file descriptor\n"
# define WAITPID_ERROR "Failed to wait for child process to end\n"
# define FORK_FAIL "Failed to dublicate a process\n"
# define PIPE_FAIL "Failed to create a pipe\n"
# define FILE_ARG ": filename argument required\n"

extern sig_atomic_t	g_signal;

enum e_success
{
	SUCCESS,
	FAILURE
};

enum e_token_type
{
	COMM = 1,
	ARG,
	REDIR_INPUT,
	REDIR_OUTPUT,
	DELIM,
	INPUT,
	OUTPUT,
	PIPE,
	FLAG,
	SPACES
};

enum e_quotes
{
	S_QUOTE = 1,
	D_QUOTE
};

typedef struct s_envp
{
	char			*line;
	struct s_envp	*next;
}	t_envp;

typedef struct s_token
{
	char			*line;
	int				level;
	int				type;
	int				token_number;
	int				expanded;
	int				quotes;
	struct s_token	*next;
}	t_token;

typedef struct s_exec
{
	int		**pipe;
	int		in;
	int		out;
	int		status;
	int		pipe_num;
	int		pipe_flag;
	int		**here_doc_pipe;
	int		here_doc_num;
	int		here_doc_closed;
	int		error_node_index;
	int		export_exit;
	int		orig_in;
	int		orig_out;
	char	*new_path;
	char	**param;
	pid_t	*pid;
}	t_exec;

typedef struct s_shell
{
	t_envp	*envp_copy;
	char	**envp_str;
	t_envp	*path;
	char	*uname;
	char	*pwd;
	char	*home;
	char	*prompt;
	t_exec	*exec;
	char	*user_input;
	t_token	*token_pointer;
	int		single_quote;
	int		double_quote;
	int		only_one_builtin;
	int		in_child;
	int		child_exit_code;
	int		in_here_doc;
	int		exit_code;
}	t_shell;

// FUNCTIONS
// reading input
int		read_input(t_shell *shell);
int		input_error_check(t_shell *shell);
// list envp functions
int		copy_envp(t_envp **envp_copy, char *envp[]);
void	ft_lstadd_back_envp(t_envp **lst, t_envp *new);
t_envp	*ft_lstnew_envp(char *content);
t_envp	*ft_lstnew_envp_no_strdup(char *content);
t_envp	*ft_lstlast_envp(t_envp *lst);
int		copy_path(t_envp **path, t_envp *envp_copy);
int		copy_uname(char **uname, t_envp *envp_copy);
int		copy_home(char **home, t_envp *envp_copy);
int		update_pwd(t_envp **envp_copy, t_shell *shell);
int		update_old_pwd(t_envp **envp_copy, t_shell *shell);
int		update_pwd(t_envp **envp_copy, t_shell *shell);
int		update_old_pwd(t_envp **envp_copy, t_shell *shell);
void	envp_remove_if_line(t_envp **lst, char *data, int (*cmp)());
void	envp_remove_if_export(t_envp **lst, char *data, int (*cmp)());
void	delete_envp(t_envp **copy_envp);
void	delete_envp_part(t_envp **copy_envp, int i);
char	choose_char(char *data);
t_envp	*find_envp_line(t_envp *envp, char *line);
t_envp	*find_envp_line_simple(t_envp *envp, char *line);
// list token functions
void	ft_lstadd_back_token(t_token **lst, t_token *new);
t_token	*ft_lstnew_token(char *content);
t_token	*ft_lstlast_token(t_token *lst);
void	delete_one_token(t_token *lst);
void	delete_all_tokens(t_token **lst);
// builtin functions
int		exec_builtins(t_shell *shell, int loop);
int		cd_exec(t_shell *shell, t_token *cd, int loop_count);
char	*get_pwd(char *home, t_shell *shell);
int		echo(t_shell *shell, t_token *echo_pointer);
int		env_exec(t_envp *envp, t_shell *shell, t_token *env, int loop);
int		unset_exec(t_envp **envp_copy, t_token *unset);
int		exit_exec(t_shell *shell, t_token *token);
int		export_exec(t_envp **envp, t_token *exp, int loop, t_shell *shell);
int		if_builtin(t_shell *shell, int loop_count);
char	*get_new_path(t_shell *shell, t_token *arg);
int		is_directory_new(char *path);
char	*remove_plus(char *data);
char	*append_export_line(char *existing, char *to_append);
int		check_str(char *str, int minus, int plus, int equal);
int		only_digits_or_empty(char *str);
int		pwd_exec(t_shell *shell);
int		forbidden_symbols(char *str);
// parsing functions
void	tokenize_input(t_shell *shell);
int		is_io(char c);
int		is_quote(char c);
int		is_space(char c);
int		is_valid_redir(t_shell *shell, int index1, int index2);
void	reset_quotes(t_shell *shell);
void	assign_type(t_token **token);
void	assign_level(t_token **token, t_exec **exec, t_shell *shell);
int		fill_values_before_prompt(t_shell **shell);
// expander functions
void	expander(t_shell *shell);
void	check_content(t_shell *shell, t_token *token, int here_doc);
char	*expansion_loop(t_shell *shell, t_token *token, \
	int index, int here_doc);
char	*create_expansion(t_shell *shell, t_token *token, \
	char **replacement, int *index);
int		calculate_key_len(t_token *token, int index);
char	*find_variable(t_shell *shell, char *key, int len);
char	*expand_variable(t_shell *shell, t_token *token, \
	char **replacement, char *ptr);
void	add_expansion(char **replace, char **exp, int *copy_index, int *index);
char	*init_replacement(t_shell *shell, t_token *token);
int		handle_quotes(t_shell *shell, t_token *token, int index);
int		is_exception(t_shell *shell, t_token *token, int index);
char	*find_exit_value(t_shell *shell, int *index);
void	realloc_replacement(t_shell *shell, t_token *token, \
	char **replacement, char *expansion);
void	expand_tilde(t_shell *shell, t_token *token, int here_doc);
char	*safe_strdup(t_shell *shell, char *arg);
void	expand_here_doc(t_shell *shell, char **input);
// syntax check
int		count_io(t_shell *shell, int index);
int		check_consecutive_io(t_shell *shell, int index);
int		check_redir_location(t_shell *shell, int index);
int		check_pipe_location(t_shell *shell, int old_index);
// execute functions
int		execute(t_shell *shell);
int		get_input_and_output(t_shell **shell, int loop);
void	check_param(t_shell *shell, int loop_count);
char	**param_to_arr(t_token *token, int loop_count);
char	*check_path(t_envp *paths, char **param, t_shell *shell);
void	check_command_access(char **param, t_shell *shell);
int		is_directory(char *path, t_shell *shell);
int		is_file(char *path);
void	pipe_and_fork(t_shell *shell, int loop);
char	**envp_to_arr(t_envp *envp_copy);
void	child_process(t_shell **shell, int loop_count);
void	close_pipes_child(int loop_count, t_exec **exec);
void	assign_exec_values(t_shell *shell);
int		here_doc(t_shell *shell);
t_token	*find_here_doc_token(t_token *token);
int		free_here_doc(char *line, char *input, int null_return, t_shell *shell);
int		check_for_input(t_shell *shell, t_token *token, int loop, int input);
int		check_for_output(t_shell *shell, t_token *token, int loop, int output);
void	allocate_here_doc(t_exec *exec, t_shell *shell);
void	check_file_access(t_shell *shell, char	*path, int loop_count);
void	check_all_files(t_token *token, t_exec *exec, int loop, t_shell *shell);
int		check_out_no_recur(t_shell *s, t_token *t, int loop, int error_node);
t_token	*find_token_index(t_token *token, int loop_count, \
	int token_type, int error_node);
int		out_folder(char *path, t_shell *shell);
void	close_pipes_parent(t_exec **exec);
int		open_file(t_shell *shell, t_token *temp, char *outfile, int loop);
int		check_for_here_doc(t_shell *shell, t_token *token, int loop);
int		change_shlvl(t_envp **envp, int add, t_shell *shell);
char	*append_shlvl_line(char *line, int add, t_shell *shell);
// miscellaneous
t_token	*find_token(t_token *token, int loop_count, int token_type);
t_token	*find_token_line(t_token *token, int loop, int token_type, char *line);
int		count_nodes_type(t_token *start, int token_type, int loop_count);
int		ft_split_list(t_envp **path, char const *s, char c);
char	*ft_strjoin_four(char *s1, char *s2, char *s3, char *s4);
size_t	ft_strchr_index(const char *s, int c);
// signals
void	set_up_signals(t_shell *shell);
void	clear_input_normal(int signal);
void	clear_input_subprocess(int signal);
int		null_signal(t_shell *shell, char *arg);
void	quit_process(int signal);
void	handle_readline_signal(t_shell *shell);
// exit
void	free_and_exit(t_shell *shell, int error);
void	free_double_arr(char ***arr);
void	error_printer(t_shell *shell, char *arg, char *message, int exit);
int		free_two_str(char **str1, char **str2);
int		free_str(char **str1);
int		free_exec(t_exec **exec);
void	free_shell(t_shell **shell, int free_envp);
char	**free_double_arr_part(char ***arr, int count);

#endif