//42 header

#include "../inc/minishell.h"

void	printing(char *cmd, char *dest, char *result, int fd)
{
	//ft_putstr_fd("pipex: ", fd);
	ft_putstr_fd(cmd, fd);
	ft_putstr_fd(dest, fd);
	ft_putstr_fd(result, fd);
}