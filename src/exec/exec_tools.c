//42 header

#include "../../inc/minishell.h"

static int	count_str(t_token *token, int loop_count);

int	is_directory(char *path, t_exec fd, int fd_pipe, char **param)
{
	int		test_fd;
	char	buffer;
	ssize_t	result;

	(void)fd_pipe;
	(void)param;
	(void)fd;
	test_fd = open(path, O_RDONLY);
	if (test_fd < 0)
		return (-1);
	result = read(test_fd, &buffer, 1);
	if (result < 0 && errno == EISDIR)
	{
		printing(path, "", ": Is a directory\n", 2);
/* 		close_free(test_fd, fd_pipe, -1, &fd.pid); // when do we need to close pipes?
		free_all(param, NULL, NULL, &fd.null); */ // do we need to free pids?
		exit(1);
	}
	close(test_fd);
	return (0);
}

char	**param_to_arr(t_token *token, int loop_count)
{
	char	**param_arr;
	t_token	*temp;
	int		i;

	param_arr = (char **)malloc((count_str(token, loop_count) + 1) * sizeof(char *));
	if (param_arr == NULL)
		return ((void *) 0);
	temp = find_token(token, loop_count, COMM);
	i = 0;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == COMM || temp->type == ARG)
		{
			param_arr[i] = ft_strdup(temp->line);
			if (param_arr[i] == NULL)
				return (NULL/* free_m(param_arr, i) */);
			i++;
		}
		temp = temp->next;
	}
	param_arr[i] = ((void *) 0);
	return (param_arr);
}

static int	count_str(t_token *token, int loop_count)
{
	t_token	*temp;
	int		count;

	temp = find_token(token, loop_count, COMM);
	count = 0;
	while (temp && temp->level == loop_count)
	{
		if (temp->type == COMM || temp->type == ARG)
			count++;
		temp = temp->next;
	}
	return (count);
}

void	check_command_access(char **param, t_exec exec)
{
	char	*command;

	(void)exec;
	command = param[0];
	if (access(command, F_OK) == -1 && errno == ENOENT)
	{
		printing(command, "", ": No such file or directory\n", 2);
		/* free_all(param, NULL, NULL, &fd.pid); */
		exit(127);
	}
}