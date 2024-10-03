//42 header

#include "../../inc/minishell.h"

static char	*get_hd_input(char *delim);

static char		*new_hd_input(char **hd_input, char **new_line);

static char		*add_hd_memory(char *str, size_t add_len);

//The function implements the scenario of here_doc
int	here_doc(t_exec *exec, t_token *redir)
{
	char	*hd_input;
	char	*delim;
	
	/* if (get_delimiter(argv, fd) == 1)
		return (1);
	fd->cmd_num = argc - 4;
	fd->hd_flag = 1; */
	delim = redir->next->line;
	hd_input = get_hd_input(delim);
	if (!hd_input)
		return (1);
	if (pipe(exec->hd_pipe) == -1)
	{
		perror("Pipe failed");
		return (free_two_str(hd_input, NULL));
	}
	ft_putstr_fd(hd_input, exec->hd_pipe[1]);
	free_two_str(hd_input, NULL);
	close(exec->hd_pipe[1]);
	exec->in = exec->hd_pipe[0];
	exec->hd_flag = 1;
	return (0);
}

//The function reads the user input with the help of get_next_line tool
static char	*get_hd_input(char *delim)
{
	char	*new_line;
	char	*hd_input;

	new_line = NULL;
	hd_input = ft_strdup("");
	if (hd_input == NULL)
	{
		perror("malloc() failed");
		return (NULL);
	}
	ft_printf("> ");
	new_line = get_next_line(0);
	while (new_line)
	{
		if (!ft_strncmp(new_line, delim, ft_strlen(delim)) && \
			ft_strchr_fix(new_line, '\n') == ft_strlen(delim))
			break ;
		if (new_hd_input(&hd_input, &new_line) == NULL)
		{
			free_two_str(new_line, NULL); // free hd_input?
			return (NULL);
		}
	}
	free_two_str(NULL, new_line);
	return (hd_input);
}

//The function concatenates the string read from the new line
//to the input from previous lines
static char	*new_hd_input(char **hd_input, char **new_line)
{
	*hd_input = add_hd_memory(*hd_input, ft_strlen(*new_line));
	if (*hd_input == NULL)
	{
		perror("malloc() failed");
		return (NULL);
	}
	*hd_input = ft_strncat(*hd_input, *new_line, \
			ft_strlen(*hd_input) + ft_strlen(*new_line) + 1);
	free_two_str(*new_line, NULL);
	ft_printf("> ");
	*new_line = get_next_line(0);
	return ("");
}

//The function allocates new memory to store user input from command line
static char	*add_hd_memory(char *str, size_t add_len)
{
	char	*temp;
	size_t	old_len;

	if (str == NULL)
		str = ft_strdup("");
	if (str == NULL)
		return (NULL);
	old_len = ft_strlen(str);
	temp = (char *)malloc(old_len + add_len + 1);
	if (temp == NULL)
		return (NULL);
	while (*str)
	{
		*temp = *str;
		temp++;
		str++;
	}
	*temp = '\0';
	free(str - old_len);
	return (temp - old_len);
}